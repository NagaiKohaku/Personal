#include "Model.h"

#include "Base/DirectXCommon.h"
#include "2d/Sprite/TextureManager.h"

#include "Math/MakeMatrixMath.h"

#include <fstream>
#include <sstream>
#include <cassert>

#include "3d/Mesh/ModelMesh.h"

///=====================================================/// 
/// モデルを初期化 (モデルデータ読み込み)
///=====================================================///
void Model::Initialize(const std::string& directoryPath, const std::string& filename) {

	/// === モデル基底のインスタンスを取得 === ///

	// モデル基底のインスタンスを取得
	modelCommon_ = ModelCommon::GetInstance();

	/// === メッシュの作成 === ///

	// ModelMesh のインスタンスを生成
	modelData_.mesh = std::make_unique<ModelMesh>();

	// OBJ 読み込み（usemtl / mtllib を処理）
	LoadObjFile(directoryPath, filename);

	// メッシュの初期化
	modelData_.mesh->Initialize();

	//メッシュの更新
	modelData_.mesh->UpdateMeshDataGPU();

	/// === マテリアルの設定 === ///

	for (size_t i = 0; i < modelData_.texturePaths.size(); ++i) {

		auto buf = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

		materialResources_.push_back(buf);

		Material* mapped = nullptr;

		buf->Map(0, nullptr, reinterpret_cast<void**>(&mapped));

		// initialize defaults
		mapped->color = Vector4(1, 1, 1, 1);

		mapped->enableLighting = 1;

		mapped->uvTransform = MakeIdentity4x4();

		mapped->shininess = 50.0f;

		mapped->environmentCoefficient = 1.0f;

		materialDataMapped_.push_back(mapped);

		// ensure texture loaded into TextureManager
		TextureManager::GetInstance()->LoadTexture(modelData_.texturePaths[i]);
	}

}

///=====================================================///
/// モデルを初期化 (メッシュクラスから生成)
/// =====================================================///
void Model::Initialize(MeshType type, const std::string& textureFilePath) {

	modelCommon_ = ModelCommon::GetInstance();

	// ModelMesh のインスタンスを生成
	modelData_.mesh = CreateMesh(type);

	// メッシュの初期化
	modelData_.mesh->Initialize();

	modelData_.subMeshes["Primitive"] = Submesh();
	modelData_.subMeshes["Primitive"].materialIndex = 0;
	modelData_.subMeshes["Primitive"].startIndex = 0;
	modelData_.subMeshes["Primitive"].indexCount = static_cast<uint32_t>(modelData_.mesh->GetIndexData().size());

	// create default single material
	modelData_.texturePaths.push_back(textureFilePath);

	// マテリアルリソースを用意（マテリアル数だけCBVを作る）
	size_t matCount = modelData_.texturePaths.size();
	if (matCount == 0) {
		// 既存1テクスチャ互換：既存処理を維持
		materialResources_.resize(1);
		materialResources_[0] = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));
		materialResources_[0]->Map(0, nullptr, reinterpret_cast<void**>(&materialDataMapped_.emplace_back(nullptr)));
		// we'll remap correctly below
	}

	// create per-material CBV and map them
	materialResources_.clear();
	materialDataMapped_.clear();
	for (size_t i = 0; i < modelData_.texturePaths.size(); ++i) {
		auto buf = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));
		materialResources_.push_back(buf);

		Material* mapped = nullptr;
		buf->Map(0, nullptr, reinterpret_cast<void**>(&mapped));
		// initialize defaults
		mapped->color = Vector4(1, 1, 1, 1);
		mapped->enableLighting = 1;
		mapped->uvTransform = MakeIdentity4x4();
		mapped->shininess = 50.0f;
		mapped->environmentCoefficient = 1.0f;

		materialDataMapped_.push_back(mapped);

		// ensure texture loaded into TextureManager
		TextureManager::GetInstance()->LoadTexture(modelData_.texturePaths[i]);
	}

}

///=====================================================/// 
/// モデルを初期化 (既存モデルからコピー)
///=====================================================///
void Model::Initialize(MeshType type, Model* model) {

	modelCommon_ = ModelCommon::GetInstance();

	modelData_.mesh = CreateMesh(type);

	modelData_.mesh->CopyMeshData(model->modelData_.mesh->GetIndexData(), model->modelData_.mesh->GetVertexData());

	modelData_.mesh->Initialize();

	modelData_.subMeshes = model->modelData_.subMeshes;

	modelData_.materialNames = model->modelData_.materialNames;

	modelData_.texturePaths = model->modelData_.texturePaths;

	// create material CBVs for each texture path
	for (auto& path : modelData_.texturePaths) {
		auto buf = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));
		materialResources_.push_back(buf);
		Material* mapped = nullptr;
		buf->Map(0, nullptr, reinterpret_cast<void**>(&mapped));
		mapped->color = Vector4(1, 1, 1, 1);
		mapped->enableLighting = 1;
		mapped->uvTransform = MakeIdentity4x4();
		mapped->shininess = 50.0f;
		mapped->environmentCoefficient = 1.0f;
		materialDataMapped_.push_back(mapped);

		TextureManager::GetInstance()->LoadTexture(path);
	}
}

///=====================================================/// 
/// モデルを描画
///=====================================================///
void Model::Draw() {

	// メッシュの設定（VB/IB 設定）
	modelData_.mesh->SendDataForGPU();

	// サブメッシュごとに描画
	for (const auto& pair : modelData_.subMeshes) {

		std::string key = pair.first;

		Submesh subMesh = pair.second;

		// set material CBV for this submesh
		size_t matIdx = subMesh.materialIndex < materialResources_.size() ? subMesh.materialIndex : 0;
		if (!materialResources_.empty()) {
			modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResources_[matIdx]->GetGPUVirtualAddress());
		}

		// set texture SRV for this material (TextureManager returns GPU handle)
		if (!modelData_.texturePaths.empty()) {
			std::string path = modelData_.texturePaths[matIdx];
			modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(path));
		}

		// draw only that submesh index range
		modelCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(
			subMesh.indexCount, 1, subMesh.startIndex, 0, 0);
	}
}

///=====================================================/// 
/// メッシュをGPUへ送信
///=====================================================///
void Model::SendMeshDataForGPU() {
	modelData_.mesh->SendDataForGPU();
}

///=====================================================/// 
/// マテリアルをGPUへ送信（単体使用時は個別CBVを更新）
///=====================================================///
void Model::SendMaterialDataForGPU() {
	// if single material expected, bind first CBV
	if (!materialResources_.empty()) {
		modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResources_[0]->GetGPUVirtualAddress());
	}
}

///=====================================================/// 
/// テクスチャをGPUへ送信（互換用）
///=====================================================///
void Model::SendTextureDataForGPU() {
	if (!modelData_.texturePaths.empty()) {
		modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData_.texturePaths[0]));
	}
}

///=====================================================/// 
/// サブメッシュのローカル変換を設定
///=====================================================///
void Model::SetSubmeshLocalTransform(size_t submeshIndex, const Matrix4x4& localTransform) {
	if (submeshIndex >= modelData_.subMeshes.size()) return;
	modelData_.subMeshes[submeshIndex].localTransform = localTransform;
}

///=====================================================/// 
/// サブメッシュのローカル変換を取得
///=====================================================///
Matrix4x4 Model::GetSubmeshLocalTransform(size_t submeshIndex) const {
	if (submeshIndex >= modelData_.subMeshes.size()) return MakeIdentity4x4();
	return modelData_.subMeshes[submeshIndex].localTransform;
}

///=====================================================/// 
/// CPUでサブメッシュの変換を反映して頂点バッファを更新する
/// 注意: 重い処理なので必要なときだけ呼んでください。
///=====================================================///
void Model::UpdateSubmeshTransformsCPU() {
	// if no baseVertices stored, initialize it from current vertices
	if (modelData_.baseVertices.size() != modelData_.mesh->GetVertexData().size()) {
		modelData_.baseVertices = modelData_.mesh->GetVertexData();
	}

	// start with base vertices
	auto currentVerts = modelData_.baseVertices;

	// For each submesh, transform the vertices referenced by its indices
	for (const auto& pair : modelData_.subMeshes) {

		std::string key = pair.first;

		Submesh subMesh = pair.second;

		// build transform matrix (for now assume world = local)
		Matrix4x4 m = subMesh.localTransform;

		// iterate indices for this submesh
		uint32_t start = subMesh.startIndex;
		uint32_t count = subMesh.indexCount;

		for (uint32_t i = 0; i < count; ++i) {
			uint32_t idx = modelData_.mesh->GetIndexData()[start + i];
			// transform position (assume position is Vector4)
			Vector4 p = currentVerts[idx].position;
			// apply matrix (assuming row-major helper functions exist)
			// We'll multiply as float4 = mul(matrix, float4)
			// Here we implement a simple multiply
			Vector4 np;
			np.x = p.x * m.m[0][0] + p.y * m.m[1][0] + p.z * m.m[2][0] + p.w * m.m[3][0];
			np.y = p.x * m.m[0][1] + p.y * m.m[1][1] + p.z * m.m[2][1] + p.w * m.m[3][1];
			np.z = p.x * m.m[0][2] + p.y * m.m[1][2] + p.z * m.m[2][2] + p.w * m.m[3][2];
			np.w = p.x * m.m[0][3] + p.y * m.m[1][3] + p.z * m.m[2][3] + p.w * m.m[3][3];
			currentVerts[idx].position = np;

			// transform normal (ignore translation): assume normal is Vector3
			Vector3 n = currentVerts[idx].normal;
			Vector3 nn;
			nn.x = n.x * m.m[0][0] + n.y * m.m[1][0] + n.z * m.m[2][0];
			nn.y = n.x * m.m[0][1] + n.y * m.m[1][1] + n.z * m.m[2][1];
			nn.z = n.x * m.m[0][2] + n.y * m.m[1][2] + n.z * m.m[2][2];
			currentVerts[idx].normal = nn;
		}
	}

	modelData_.mesh->CopyMeshData(modelData_.mesh->GetIndexData(), currentVerts);

	modelData_.mesh->UpdateMeshDataGPU();
}

///=====================================================/// 
/// OBJファイルの読み込み（usemtl 対応）
///=====================================================///
void Model::LoadObjFile(const std::string& directoryPath, const std::string& filename) {

	//ファイルの内容
	std::ifstream file(directoryPath + "/" + filename);

	//ファイルの1行分の内容
	std::string line;

	//3頂点データ
	MeshBase::VertexData triangle[3];

	//頂点座標データ
	std::vector<Vector4> positions;

	//頂点方向データ
	std::vector<Vector3> normals;

	//UV座標データ
	std::vector<Vector2> texcoords;

	//オブジェクト名
	std::string objectName;

	//ファイルを開く
	assert(file.is_open());

	// 現在のサブメッシュの数
	int currentSubmesh = 0;

	//1行ずつ読み込む
	while (std::getline(file, line)) {

		//読み込んだ内容
		std::istringstream s(line);

		//識別子
		std::string identifier;

		//識別子要素を読み込む
		s >> identifier;

		if (identifier == "v") {

			/// === 頂点座標データの読み込み === ///

			//頂点座標データ
			Vector4 position;

			//頂点座標データを読み込む
			s >> position.x >> position.y >> position.z;

			//DirectXの設定に合わせてX軸を反転
			position.x *= -1.0f;

			//同次座標を設定
			position.w = 1.0f;

			//頂点座標データを登録
			positions.push_back(position);

		} else if (identifier == "vt") {

			/// === uv座標データの読み込み === ///

			//uv座標データ
			Vector2 texCoord;

			//uv座標データを読み込む
			s >> texCoord.x >> texCoord.y;

			//DirectXの設定に合わせてy軸を調整
			texCoord.y = 1.0f - texCoord.y;

			//uv座標データを登録
			texcoords.push_back(texCoord);

		} else if (identifier == "vn") {

			/// === 頂点方向データの読み込み === ///

			//頂点方向データ
			Vector3 normal;

			//頂点方向データを読み込む
			s >> normal.x >> normal.y >> normal.z;

			//DirectXの設定に合わせてX軸を反転
			normal.x *= -1.0f;

			//頂点方向データを登録
			normals.push_back(normal);

		} else if (identifier == "o") {

			/// === サブメッシュの生成 === ///

			//オブジェクト名を読み込む
			s >> objectName;

			//サブメッシュを生成
			modelData_.subMeshes[objectName] = Submesh();

		} else if (identifier == "f") {

			/// === 三角面情報の読み込み === ///

			for (int faceVertex = 0; faceVertex < 3; ++faceVertex) {

				//頂点定義
				std::string vertexDefinition;

				//頂点定義を読み込む
				s >> vertexDefinition;

				std::istringstream v(vertexDefinition);

				//要素番号
				uint32_t elementIndices[3] = { 0,0,0 };

				//3頂点分の要素番号を読み込む
				for (int element = 0; element < 3; ++element) {

					std::string index;

					std::getline(v, index, '/');

					elementIndices[element] = index.empty() ? 0 : std::stoi(index);
				}

				//要素番号から頂点座標、頂点方向、uv座標を取得する
				Vector4 position = positions[elementIndices[0] - 1];

				Vector2 texCoord = texcoords[elementIndices[1] - 1];

				Vector3 normal = normals[elementIndices[2] - 1];

				//1頂点ずつデータを登録
				triangle[faceVertex] = { position, texCoord, normal };
			}

			for (int i = 2; i >= 0; i--) {

				//メッシュに頂点データを設定する
				modelData_.mesh->AddVertexData(triangle[i]);

				//頂点番号を増やす
				modelData_.subMeshes[objectName].indexCount += 1;
			}

		} else if (identifier == "mtllib") {

			/// === マテリアルの読み込み === ///

			//マテリアルファイル名
			std::string materialFilename;

			//マテリアルファイル名を読み込む
			s >> materialFilename;

			//マテリアル情報を読み込む
			LoadMaterialTemplateFile(directoryPath, materialFilename);

		} else if (identifier == "usemtl") {

			/// === マテリアルの設定 === ///

			//マテリアル名
			std::string matName;

			//マテリアル名を読み込む
			s >> matName;

			//マテリアル名が存在するかチェック
			auto it = std::find(modelData_.materialNames.begin(), modelData_.materialNames.end(), matName);

			int matIndex = -1;

			if (it == modelData_.materialNames.end()) {

				//マテリアル番号を振り分け
				matIndex = static_cast<int>(modelData_.materialNames.size());

				//モデルデータにマテリアル名を登録
				modelData_.materialNames.push_back(matName);

				//テクスチャパスが存在するかチェック
				auto tIt = mtlToTextureMap_.find(matName);

				if (tIt != mtlToTextureMap_.end()) {

					//モデルデータにテクスチャパスを登録
					modelData_.texturePaths.push_back(tIt->second);
				} else {

					//空の文字列で登録
					modelData_.texturePaths.push_back(std::string());
				}

				modelData_.subMeshes[objectName].materialIndex = matIndex;

				modelData_.subMeshes[objectName].startIndex = uint32_t(modelData_.mesh->GetIndexData().size());

				modelData_.subMeshes[objectName].indexCount = 0;
			} else {

				matIndex = int(std::distance(modelData_.materialNames.begin(), it));

				// start a new submesh for this material
				modelData_.subMeshes[objectName].materialIndex = matIndex;

				modelData_.subMeshes[objectName].startIndex = uint32_t(modelData_.mesh->GetIndexData().size());

				modelData_.subMeshes[objectName].indexCount = 0;
			}

			currentSubmesh = int(modelData_.subMeshes.size()) - 1;
		}
	}
}

///=====================================================/// 
/// MTLファイルを読み込み、マテリアル名->テクスチャパスのマップを作る
///=====================================================///
void Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {

	//ファイルの内容
	std::ifstream file(directoryPath + "/" + filename);

	//ファイルの1行分の内容
	std::string line;

	//マテリアル名
	std::string materialName;

	//テクスチャファイル名
	std::string textureFilename;

	//ファイルを開く
	assert(file.is_open());

	//ファイルを読み込む
	while (std::getline(file, line)) {

		//読み込んだ内容
		std::istringstream s(line);

		//識別子
		std::string identifier;

		//識別子要素を読み込む
		s >> identifier;

		if (identifier == "newmtl") {

			//マテリアル名を読み込む
			s >> materialName;

		} else if (identifier == "map_Kd") {

			//テクスチャファイル名を読み込む
			s >> textureFilename;

			//マテリアル名とテクスチャファイル名を登録
			mtlToTextureMap_[materialName] = directoryPath + "/" + textureFilename;
		}
	}
}

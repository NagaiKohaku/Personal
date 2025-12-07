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

	// モデル基底のインスタンスを取得
	modelCommon_ = ModelCommon::GetInstance();

	// OBJ 読み込み（usemtl / mtllib を処理）
	LoadObjFile(directoryPath, filename);

	// Mesh の生成（1つの VB/IB にまとめる設計のまま）
	mesh_ = std::make_unique<ModelMesh>();

	// 頂点数の設定
	mesh_->SetVertexCount(uint32_t(modelData_.vertices.size()));

	// インデックス数の設定
	mesh_->SetIndexCount(uint32_t(modelData_.indices.size()));

	// 初期化
	mesh_->Initialize();

	// 頂点データとインデックスデータのコピー
	mesh_->CopyMeshData(modelData_.indices, modelData_.vertices);

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
		mapped->color = Vector4(1,1,1,1);
		mapped->enableLighting = 1;
		mapped->uvTransform = MakeIdentity4x4();
		mapped->shininess = 50.0f;
		mapped->environmentCoefficient = 1.0f;

		materialDataMapped_.push_back(mapped);

		// ensure texture loaded into TextureManager
		TextureManager::GetInstance()->LoadTexture(modelData_.texturePaths[i]);
	}

	// Backwards compatibility: if single texture present, set textureFilePath_
	if (!modelData_.texturePaths.empty()) {
		textureFilePath_ = modelData_.texturePaths[0];
	}
}

///=====================================================///
/// モデルを初期化 (メッシュクラスから生成)
/// =====================================================///
void Model::Initialize(MeshType type, const std::string& textureFilePath) {

	modelCommon_ = ModelCommon::GetInstance();

	mesh_ = CreateMesh(type);
	mesh_->Initialize();

	if (modelData_.submeshes.empty()) {
		modelData_.submeshes.push_back(Submesh());
		modelData_.submeshes[0].startIndex = 0;
		modelData_.submeshes[0].indexCount = (uint32_t)modelData_.indices.size();
		modelData_.submeshes[0].materialIndex = 0;
	}

	for (uint32_t i = 0; i < mesh_->GetVertexCount(); i++) {
		modelData_.vertices.push_back(mesh_->GetVertexData()[i]);
	}
	for (uint32_t i = 0; i < mesh_->GetIndexCount(); i++) {
		modelData_.indices.push_back(mesh_->GetIndexData()[i]);
		modelData_.submeshes[0].indexCount++;
	}

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

	// Backwards compatibility: if single texture present, set textureFilePath_
	if (!modelData_.texturePaths.empty()) {
		textureFilePath_ = modelData_.texturePaths[0];
	}
}

///=====================================================/// 
/// モデルを初期化 (既存モデルからコピー)
///=====================================================///
void Model::Initialize(MeshType type, Model* model) {

	modelCommon_ = ModelCommon::GetInstance();

	// copy
	Copy(model);

	// mesh create and copy as before
	mesh_ = CreateMesh(type);
	mesh_->SetVertexCount(uint32_t(modelData_.vertices.size()));
	mesh_->SetIndexCount(uint32_t(modelData_.indices.size()));
	mesh_->Initialize();
	mesh_->CopyMeshData(modelData_.indices, modelData_.vertices);

	// create material CBVs for each texture path
	for (auto& path : modelData_.texturePaths) {
		auto buf = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));
		materialResources_.push_back(buf);
		Material* mapped = nullptr;
		buf->Map(0, nullptr, reinterpret_cast<void**>(&mapped));
		mapped->color = Vector4(1,1,1,1);
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
	mesh_->SendDataForGPU();

	// サブメッシュごとに描画
	for (const auto& sm : modelData_.submeshes) {

		// set material CBV for this submesh
		size_t matIdx = sm.materialIndex < materialResources_.size() ? sm.materialIndex : 0;
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
			sm.indexCount, 1, sm.startIndex, 0, 0);
	}
}

///=====================================================/// 
/// メッシュをGPUへ送信
///=====================================================///
void Model::SendMeshDataForGPU() {
	mesh_->SendDataForGPU();
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
/// モデルのデータをコピー
///=====================================================///
void Model::Copy(Model* model) {
	modelData_ = model->modelData_;
	textureFilePath_ = model->textureFilePath_;
}

///=====================================================/// 
/// サブメッシュのローカル変換を設定
///=====================================================///
void Model::SetSubmeshLocalTransform(size_t submeshIndex, const Matrix4x4& localTransform) {
	if (submeshIndex >= modelData_.submeshes.size()) return;
	modelData_.submeshes[submeshIndex].localTransform = localTransform;
}

///=====================================================/// 
/// サブメッシュのローカル変換を取得
///=====================================================///
Matrix4x4 Model::GetSubmeshLocalTransform(size_t submeshIndex) const {
	if (submeshIndex >= modelData_.submeshes.size()) return MakeIdentity4x4();
	return modelData_.submeshes[submeshIndex].localTransform;
}

///=====================================================/// 
/// CPUでサブメッシュの変換を反映して頂点バッファを更新する
/// 注意: 重い処理なので必要なときだけ呼んでください。
///=====================================================///
void Model::UpdateSubmeshTransformsCPU() {
	// if no baseVertices stored, initialize it from current vertices
	if (modelData_.baseVertices.size() != modelData_.vertices.size()) {
		modelData_.baseVertices = modelData_.vertices;
	}

	// start with base vertices
	auto currentVerts = modelData_.baseVertices;

	// For each submesh, transform the vertices referenced by its indices
	for (const auto& sm : modelData_.submeshes) {
		// build transform matrix (for now assume world = local)
		Matrix4x4 m = sm.localTransform;

		// iterate indices for this submesh
		uint32_t start = sm.startIndex;
		uint32_t count = sm.indexCount;

		for (uint32_t i = 0; i < count; ++i) {
			uint32_t idx = modelData_.indices[start + i];
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

	// copy modified vertices into GPU buffer via mesh_->CopyMeshData
	mesh_->CopyMeshData(modelData_.indices, currentVerts);

	// also update stored 'vertices' copy
	modelData_.vertices = std::move(currentVerts);
}

///=====================================================/// 
/// OBJファイルの読み込み（usemtl 対応）
///=====================================================///
void Model::LoadObjFile(const std::string& directoryPath, const std::string& filename) {

	// 一時データ
	MeshBase::VertexData triangle[3];
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;

	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	// current submesh index in modelData_.submeshes
	int currentSubmesh = -1;

	while (std::getline(file, line)) {
		std::istringstream s(line);
		std::string identifier;
		s >> identifier;
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.x *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		} else if (identifier == "vt") {
			Vector2 texCoord;
			s >> texCoord.x >> texCoord.y;
			texCoord.y = 1.0f - texCoord.y;
			texcoords.push_back(texCoord);
		} else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			normals.push_back(normal);
		} else if (identifier == "f") {
			// Ensure we have a submesh to push into
			if (currentSubmesh < 0) {
				// create a default submesh 0
				modelData_.submeshes.push_back(Submesh());
				modelData_.submeshes.back().startIndex = 0;
				modelData_.submeshes.back().indexCount = 0;
				modelData_.materialNames.push_back(std::string("default"));
				modelData_.texturePaths.push_back(textureFilePath_); // may be empty
				currentSubmesh = 0;
			}

			for (int faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3] = {0,0,0};
				for (int element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = index.empty() ? 0 : std::stoi(index);
				}
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texCoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				triangle[faceVertex] = { position, texCoord, normal };
			}

			for (int i = 2; i >= 0; i--) {
				modelData_.indices.push_back(uint32_t(modelData_.vertices.size()));
				modelData_.vertices.push_back(triangle[i]);
				modelData_.submeshes[currentSubmesh].indexCount += 1;
			}

		} else if (identifier == "mtllib") {
			std::string materialFilename;
			s >> materialFilename;
			LoadMaterialTemplateFile(directoryPath, materialFilename);
		} else if (identifier == "usemtl") {
			std::string matName;
			s >> matName;
			// determine material index (add if new)
			auto it = std::find(modelData_.materialNames.begin(), modelData_.materialNames.end(), matName);
			int matIndex = -1;
			if (it == modelData_.materialNames.end()) {
				matIndex = int(modelData_.materialNames.size());
				modelData_.materialNames.push_back(matName);
				// push texture path if known from parsed MTL, else empty
				auto tIt = mtlToTextureMap_.find(matName);
				if (tIt != mtlToTextureMap_.end()) modelData_.texturePaths.push_back(tIt->second);
				else modelData_.texturePaths.push_back(std::string());
				// start a new submesh record
				modelData_.submeshes.push_back(Submesh());
				modelData_.submeshes.back().startIndex = uint32_t(modelData_.indices.size());
				modelData_.submeshes.back().indexCount = 0;
				modelData_.submeshes.back().materialIndex = matIndex;
			} else {
				matIndex = int(std::distance(modelData_.materialNames.begin(), it));
				// start a new submesh for this material
				modelData_.submeshes.push_back(Submesh());
				modelData_.submeshes.back().startIndex = uint32_t(modelData_.indices.size());
				modelData_.submeshes.back().indexCount = 0;
				modelData_.submeshes.back().materialIndex = matIndex;
			}
			currentSubmesh = int(modelData_.submeshes.size()) - 1;
		}
	}

	// If we ended without create any submesh but have indices, create default
	if (modelData_.submeshes.empty() && !modelData_.indices.empty()) {
		modelData_.submeshes.push_back(Submesh());
		modelData_.submeshes.back().startIndex = 0;
		modelData_.submeshes.back().indexCount = uint32_t(modelData_.indices.size());
		modelData_.materialNames.push_back(std::string("default"));
		modelData_.texturePaths.push_back(textureFilePath_);
	}
}

///=====================================================/// 
/// MTLファイルを読み込み、マテリアル名->テクスチャパスのマップを作る
///=====================================================///
void Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {

	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	std::string line;
	std::string currentNewmtl;

	while (std::getline(file, line)) {
		std::istringstream s(line);
		std::string identifier;
		s >> identifier;
		if (identifier == "newmtl") {
			s >> currentNewmtl;
		} else if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			if (!currentNewmtl.empty()) {
				mtlToTextureMap_[currentNewmtl] = directoryPath + "/" + textureFilename;
			} else {
				// fallback: if no material name context, set default textureFilePath_
				textureFilePath_ = directoryPath + "/" + textureFilename;
			}
		}
	}
}

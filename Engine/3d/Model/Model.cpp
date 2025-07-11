#include "Model.h"

#include "Base/DirectXCommon.h"
#include "2d/Sprite/TextureManager.h"
#include "3d/Model/ModelCommon.h"

#include "Math/MakeMatrixMath.h"

#include "fstream"
#include "sstream"
#include "numbers"

#include "3d/Mesh/ModelMesh.h"

///=====================================================/// 
/// 初期化処理(モデル)
///=====================================================///
void Model::Initialize(const std::string& directoryPath, const std::string& filename) {

	//モデル基底のインスタンスを取得
	modelCommon_ = ModelCommon::GetInstance();

	//モデルデータの読み込み
	LoadObjFile(directoryPath, filename);

	/// === メッシュの生成 === ///

	//生成
	mesh_ = std::make_unique<ModelMesh>();

	//頂点数の設定
	mesh_->SetVertexCount(uint32_t(modelData_.vertices.size()));

	//インデックス数の設定
	mesh_->SetIndexCount(uint32_t(modelData_.indices.size()));

	//初期化
	mesh_->Initialize();

	//頂点データとインデックスデータのコピー
	mesh_->CopyMeshData(modelData_.indices, modelData_.vertices);

	/// === マテリアルリソースの生成 === ///

	//マテリアルリソースを作成
	materialResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	//書き込むためのアドレスを取得する
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	//マテリアルデータの設定
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 50.0f;

	//テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(textureFilePath_);
}

///=====================================================///
/// 初期化処理(メッシュ)
/// =====================================================///
void Model::Initialize(MeshType type, const std::string& textureFilePath) {

	//モデル基底のインスタンスを取得
	modelCommon_ = ModelCommon::GetInstance();

	/// === メッシュの生成 === ///

	//生成
	mesh_ = CreateMesh(type);

	//初期化
	mesh_->Initialize();

	//頂点データの設定
	for (uint32_t i = 0; i < mesh_->GetVertexCount(); i++) {

		modelData_.vertices.push_back(mesh_->GetVertexData()[i]);
	}

	//インデックスデータの設定
	for (uint32_t i = 0; i < mesh_->GetIndexCount(); i++) {

		modelData_.indices.push_back(mesh_->GetIndexData()[i]);
	}

	/// === マテリアルデータの生成 === ///

	//マテリアルリソースを作成
	materialResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	//書き込むためのアドレスを取得する
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	//マテリアルデータの設定
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 50.0f;

	//テクスチャファイルパスの設定
	textureFilePath_ = textureFilePath;

	//テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(textureFilePath_);
}

///=====================================================/// 
/// 初期化処理(コピー)
///=====================================================///
void Model::Initialize(MeshType type, Model* model) {

	//モデル基底のインスタンスを取得
	modelCommon_ = ModelCommon::GetInstance();

	/// === モデルデータのコピー === ///

	Copy(model);

	/// === メッシュの生成 === ///

	//生成
	mesh_ = CreateMesh(type);

	//頂点数を設定
	mesh_->SetVertexCount(uint32_t(modelData_.vertices.size()));

	//インデックス数を設定
	mesh_->SetIndexCount(uint32_t(modelData_.indices.size()));

	//初期化
	mesh_->Initialize();

	//頂点データとインデックスデータのコピー
	mesh_->CopyMeshData(modelData_.indices, modelData_.vertices);

	/// === マテリアルリソースの生成 === ///

	//マテリアルリソースを作成
	materialResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	//書き込むためのアドレスを取得する
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	//マテリアルデータの設定
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 50.0f;
}

///=====================================================/// 
/// 描画
///=====================================================///
void Model::Draw() {

	//メッシュの設定
	mesh_->Draw();

	//マテリアルデータの設定
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());

	//テクスチャデータの設定
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureFilePath_));

	//描画コマンド発行
	modelCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(mesh_->GetIndexCount()), 1, 0, 0, 0);

}

///=====================================================/// 
/// メッシュの描画
///=====================================================///
void Model::DrawMesh() {

	//メッシュの設定
	mesh_->Draw();
}

///=====================================================/// 
/// マテリアルの描画
///=====================================================///
void Model::DrawMaterial() {

	//マテリアルデータの設定
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());
}

///=====================================================/// 
/// テクスチャの描画
///=====================================================///
void Model::DrawTexture() {

	//テクスチャデータの設定
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureFilePath_));
}

///=====================================================/// 
/// モデルデータのコピー
///=====================================================///
void Model::Copy(Model* model) {

	modelData_ = model->modelData_;

	textureFilePath_ = model->textureFilePath_;
}

///=====================================================/// 
/// objファイルの読み込み
///=====================================================///
void Model::LoadObjFile(const std::string& directoryPath, const std::string& filename) {

	/// === ローカル変数 === ///

	//三角面の頂点データ
	MeshBase::VertexData triangle[3];

	//位置
	std::vector<Vector4> positions;

	//法線
	std::vector<Vector3> normals;

	//テクスチャ座標
	std::vector<Vector2> texcoords;

	//ファイルから読んだ1行を格納するもの
	std::string line;

	/// === objファイルからデータを読み込む === ///

	//ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);

	//ファイルが開けたかの確認
	assert(file.is_open());

	while (std::getline(file, line)) {

		//識別子
		std::string identifier;

		//1行
		std::istringstream s(line);

		//先頭の識別子を読む
		s >> identifier;

		if (identifier == "v") {

			/// === 識別子が「v」であれば === ///

			//座標データ
			Vector4 position;

			//ファイルから読み込む
			s >> position.x >> position.y >> position.z;

			//X軸を反転させる
			position.x *= -1.0f;

			position.w = 1.0f;

			//座標データを登録する
			positions.push_back(position);
		} else if (identifier == "vt") {

			/// === 識別子が「vt」だったら === ///

			//テクスチャ座標データ
			Vector2 texCoord;

			//ファイルから読み込む
			s >> texCoord.x >> texCoord.y;

			//
			texCoord.y = 1.0f - texCoord.y;

			//テクスチャ座標データを登録
			texcoords.push_back(texCoord);
		} else if (identifier == "vn") {

			/// === 識別子が「vn」であれば === ///

			//法線データ
			Vector3 normal;

			//ファイルから読み込む
			s >> normal.x >> normal.y >> normal.z;

			//X軸を反転
			normal.x *= -1.0f;

			//法線データを登録
			normals.push_back(normal);
		} else if (identifier == "f") {

			/// === 識別子が「f」だったら === ///

			//面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {

				//頂点の要素
				std::string vertexDefinition;

				//頂点の要素を読み込む
				s >> vertexDefinition;

				//頂点の要素へのIndexは[位置/UV/法線]で格納されているので、分類してIndexを取得する
				std::istringstream v(vertexDefinition);

				//頂点要素の格納用
				uint32_t elementIndices[3];

				for (int32_t element = 0; element < 3; ++element) {

					//要素
					std::string index;

					//区切りで要素を読み込む
					std::getline(v, index, '/');

					//要素を格納する
					elementIndices[element] = std::stoi(index);
				}

				//格納した要素から値を取り出していく
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texCoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];

				//三角形の構築
				triangle[faceVertex] = {position,texCoord,normal};
			}

			//頂点データの設定
			for (int i = 2; i >= 0; i--) {

				modelData_.indices.push_back(uint32_t(modelData_.vertices.size()));
				modelData_.vertices.push_back(triangle[i]);
			}

		} else if (identifier == "mtllib") {

			/// ===  識別子が「mtllib」だったら=== ///

			std::string materialFilename;

			//materialTemplateLibraryファイルの名前を取得する
			s >> materialFilename;

			//マテリアルデータを読み込む
			LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}
}

///=====================================================/// 
/// マテリアルデータの読み込み
///=====================================================///
void Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {

	//ファイルから読んだ1行を格納するもの
	std::string line;

	//ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);

	//開けなかったら止める
	assert(file.is_open());

	while (std::getline(file, line)) {

		//識別子
		std::string identifier;

		//1行
		std::istringstream s(line);

		//先頭から識別子を読み込む
		s >> identifier;

		if (identifier == "map_Kd") {

			/// === 識別子が「map_kd」だったら === ///

			//ファイル名
			std::string textureFilename;

			//ファイル名を読み込む
			s >> textureFilename;

			//連結してファイルパスにする
			textureFilePath_ = directoryPath + "/" + textureFilename;
		}
	}
}
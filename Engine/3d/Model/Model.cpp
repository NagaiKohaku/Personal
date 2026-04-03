#include "Model.h"

#include "Base/DirectXCommon.h"
#include "2d/Sprite/TextureManager.h"

#include <Math/Utility/MakeMatrixMath.h>

#include "fstream"
#include "sstream"
#include "numbers"

#include "3d/Mesh/ModelMesh.h"

namespace MyEngine {

	///=====================================================/// 
	/// モデルを初期化 (モデルデータ読み込み)
	///=====================================================///
	void Model::Initialize(const std::string& directoryPath, const std::string& filename, ModelCommon* modelCommonPtr) {

		//モデル基底のインスタンスを取得
		modelCommon_ = modelCommonPtr;

		//モデルデータの読み込み
		LoadObjFile(directoryPath, filename);

		/// === メッシュパーツごとの生成 === ///
		for (auto& part : meshParts_) {
			//生成
			part.mesh = std::make_unique<ModelMesh>();

			//頂点数の設定
			part.mesh->SetVertexCount(uint32_t(part.modelData.vertices.size()));

			//インデックス数の設定
			part.mesh->SetIndexCount(uint32_t(part.modelData.indices.size()));

			//初期化
			part.mesh->Initialize(modelCommon_->GetDxCommon());

			//頂点データとインデックスデータのコピー
			part.mesh->CopyMeshData(part.modelData.indices, part.modelData.vertices);

			/// === マテリアルリソースの生成 === ///

			//マテリアルリソースを作成
			part.materialResource = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

			//書き込むためのアドレスを取得する
			part.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&part.materialData));

			//マテリアルデータの設定
			part.materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			part.materialData->enableLighting = true;
			part.materialData->uvTransform = MakeIdentity4x4();
			part.materialData->shininess = 50.0f;
			part.materialData->environmentCoefficient = 1.0f;

			if (!part.textureFilePath.empty()) {
				//テクスチャの読み込み
				TextureManager::GetInstance()->LoadTexture(part.textureFilePath);
				part.textureIndex = TextureManager::GetInstance()->GetSrvIndex(part.textureFilePath);
			}
		}
	}

	///=====================================================///
	/// モデルを初期化 (メッシュクラスから生成)
	/// =====================================================///
	void Model::Initialize(MeshType type, const std::string& textureFilePath, ModelCommon* modelCommonPtr) {

		//モデル基底のインスタンスを取得
		modelCommon_ = modelCommonPtr;

		// パーツを1つ作成
		MeshPart part;
		part.materialName = "Default";
		part.textureFilePath = textureFilePath;

		/// === メッシュの生成 === ///

		//生成
		part.mesh = CreateMesh(type);

		//初期化
		part.mesh->Initialize(modelCommon_->GetDxCommon());

		//頂点データの設定
		for (uint32_t i = 0; i < part.mesh->GetVertexCount(); i++) {

			part.modelData.vertices.push_back(part.mesh->GetVertexData()[i]);
		}

		//インデックスデータの設定
		for (uint32_t i = 0; i < part.mesh->GetIndexCount(); i++) {

			part.modelData.indices.push_back(part.mesh->GetIndexData()[i]);
		}

		/// === マテリアルデータの生成 === ///

		//マテリアルリソースを作成
		part.materialResource = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

		//書き込むためのアドレスを取得する
		part.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&part.materialData));

		//マテリアルデータの設定
		part.materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		part.materialData->enableLighting = true;
		part.materialData->uvTransform = MakeIdentity4x4();
		part.materialData->shininess = 50.0f;
		part.materialData->environmentCoefficient = 1.0f;

		//テクスチャの読み込み
		TextureManager::GetInstance()->LoadTexture(part.textureFilePath);
		part.textureIndex = TextureManager::GetInstance()->GetSrvIndex(part.textureFilePath);

		// パーツを追加
		meshParts_.push_back(std::move(part));
	}

	///=====================================================/// 
	/// モデルを初期化 (既存モデルからコピー)
	///=====================================================///
	void Model::Initialize(MeshType type, Model* model, ModelCommon* modelCommonPtr) {

		//モデル基底のインスタンスを取得
		modelCommon_ = modelCommonPtr;

		/// === モデルデータのコピー === ///

		Copy(model);

		/// === メッシュパーツごとの生成 === ///
		for (auto& part : meshParts_) {
			//生成
			part.mesh = CreateMesh(type);

			//頂点数を設定
			part.mesh->SetVertexCount(uint32_t(part.modelData.vertices.size()));

			//インデックス数を設定
			part.mesh->SetIndexCount(uint32_t(part.modelData.indices.size()));

			//初期化
			part.mesh->Initialize(modelCommon_->GetDxCommon());

			//頂点データとインデックスデータのコピー
			part.mesh->CopyMeshData(part.modelData.indices, part.modelData.vertices);

			/// === マテリアルリソースの生成 === ///

			//マテリアルリソースを作成
			part.materialResource = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

			//書き込むためのアドレスを取得する
			part.materialResource->Map(0, nullptr, reinterpret_cast<void**>(&part.materialData));

			//マテリアルデータの設定
			part.materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			part.materialData->enableLighting = true;
			part.materialData->uvTransform = MakeIdentity4x4();
			part.materialData->shininess = 50.0f;
			part.materialData->environmentCoefficient = 1.0f;
		}
	}

	///=====================================================/// 
	/// モデルを描画
	///=====================================================///
	void Model::Draw() {

		for (auto& part : meshParts_) {
			//メッシュの設定
			part.mesh->SendDataForGPU();

			//マテリアルデータの設定
			modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, part.materialResource.Get()->GetGPUVirtualAddress());

			//テクスチャデータの設定
			modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(part.textureFilePath));

			//描画コマンド発行
			modelCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(part.mesh->GetIndexCount()), 1, 0, 0, 0);
		}
	}

	///=====================================================/// 
	/// モデルのメッシュデータを GPU に転送
	///=====================================================///
	void Model::SendMeshDataForGPU() {
		for (auto& part : meshParts_) {
			//メッシュの設定
			part.mesh->SendDataForGPU();
		}
	}

	///=====================================================/// 
	/// モデルのマテリアルデータを GPU に転送
	///=====================================================///
	void Model::SendMaterialDataForGPU() {
		for (auto& part : meshParts_) {
			//マテリアルデータの設定
			modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, part.materialResource.Get()->GetGPUVirtualAddress());
		}
	}

	///=====================================================/// 
	/// モデルのテクスチャデータを GPU に転送
	///=====================================================///
	void Model::SendTextureDataForGPU() {
		for (auto& part : meshParts_) {
			//テクスチャデータの設定
			modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(part.textureFilePath));
		}
	}

	///=====================================================/// 
	/// 指定したモデルのデータをコピー
	///=====================================================///
	void Model::Copy(Model* model) {

		// 全てのパーツをコピーする
		meshParts_.clear();
		for (auto& mPart : model->GetMeshParts()) {

			MeshPart newPart;

			newPart.materialName = mPart.materialName;
			newPart.textureFilePath = mPart.textureFilePath;
			newPart.textureIndex = mPart.textureIndex;
			newPart.modelData = mPart.modelData;

			// mesh, materialResource 等は Initialize 等で再構築される前提
			meshParts_.push_back(std::move(newPart));
		}
	}

	///=====================================================/// 
	/// OBJファイルを読み込み、モデルデータに格納
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

		// マテリアルごとへのテクスチャマッピング
		std::unordered_map<std::string, std::string> materialMap;

		// 現在処理中のパーツインデックス
		size_t currentPartIndex = static_cast<size_t>(-1);

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
					triangle[faceVertex] = { position,texCoord,normal };
				}

				//頂点データの設定
				for (int i = 2; i >= 0; i--) {

					if (currentPartIndex == static_cast<size_t>(-1)) {
						MeshPart defaultPart;
						defaultPart.materialName = "Default";
						meshParts_.push_back(std::move(defaultPart));
						currentPartIndex = 0;
					}

					meshParts_[currentPartIndex].modelData.indices.push_back(uint32_t(meshParts_[currentPartIndex].modelData.vertices.size()));
					meshParts_[currentPartIndex].modelData.vertices.push_back(triangle[i]);
				}

			} else if (identifier == "mtllib") {

				/// ===  識別子が「mtllib」だったら=== ///

				std::string materialFilename;

				//materialTemplateLibraryファイルの名前を取得する
				s >> materialFilename;

				//マテリアルデータを読み込む
				materialMap = LoadMaterialTemplateFile(directoryPath, materialFilename);
			} else if (identifier == "usemtl") {

				/// === 識別子が「usemtl」だったら === ///

				std::string materialName;
				s >> materialName;

				bool found = false;
				for (size_t i = 0; i < meshParts_.size(); i++) {
					if (meshParts_[i].materialName == materialName) {
						currentPartIndex = i;
						found = true;
						break;
					}
				}

				if (!found) {
					MeshPart newPart;
					newPart.materialName = materialName;
					if (materialMap.contains(materialName)) {
						newPart.textureFilePath = materialMap.at(materialName);
					}
					meshParts_.push_back(std::move(newPart));
					currentPartIndex = meshParts_.size() - 1;
				}
			}
		}
	}

	///=====================================================/// 
	/// MTLファイルを読み込み、モデルのテクスチャパスを設定
	///=====================================================///
	std::unordered_map<std::string, std::string> Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {

		std::unordered_map<std::string, std::string> materialMap;
		std::string currentMaterialName;

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

			if (identifier == "newmtl") {

				/// === 識別子が「newmtl」だったら === ///

				s >> currentMaterialName;

			} else if (identifier == "map_Kd") {

				/// === 識別子が「map_kd」だったら === ///

				//ファイル名
				std::string textureFilename;

				//ファイル名を読み込む
				s >> textureFilename;

				//連結してファイルマップに登録する
				if (!currentMaterialName.empty()) {
					materialMap[currentMaterialName] = directoryPath + "/" + textureFilename;
				}
			}
		}

		return materialMap;
	}
}
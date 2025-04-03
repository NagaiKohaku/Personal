#include "Model.h"

#include "Base/DirectXCommon.h"
#include "2d/Sprite/TextureManager.h"
#include "3d/Model/ModelCommon.h"

#include "Math/MakeMatrixMath.h"

#include "fstream"
#include "sstream"
#include "numbers"

///=====================================================/// 
/// 初期化処理
///=====================================================///
void Model::Initialize(const std::string& directoryPath, const std::string& filename) {

	//モデル基底のインスタンスを取得
	modelCommon_ = ModelCommon::GetInstance();

	//モデルデータの読み込み
	LoadObjFile(directoryPath, filename);

	/// === 頂点リソース === ///

	//頂点リソースを作成
	vertexResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());

	//リソースの先頭のアドレスを取得する
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());

	//1頂点当たりのサイズを設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//書き込むためのアドレスを取得する
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//頂点データの設定
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	/// === インデックスリソース === ///

	//インデックスリソースを作成
	indexResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * modelData_.vertices.size());

	//リソースの先頭アドレスを取得
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * modelData_.vertices.size());

	//フォーマットを設定
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//書き込むためのアドレスを取得する
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	//インデックスデータの設定
	std::memcpy(indexData_, modelData_.indexes.data(), sizeof(uint32_t) * modelData_.indexes.size());

	/// === マテリアルリソース === ///

	//マテリアルリソースを作成
	materialResource_ = modelCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	//書き込むためのアドレスを取得する
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	//マテリアルデータの設定
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 50.0f;

	//マテリアルの読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
}

///=====================================================/// 
/// 描画処理
///=====================================================///
void Model::Draw() {

	//頂点データの設定
	modelCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	//頂点番号の設定
	modelCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	//マテリアルデータの設定
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());

	//テクスチャデータの設定
	modelCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData_.material.textureFilePath));

	//描画コマンド発行
	modelCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indexes.size()), 1, 0, 0, 0);

}

///=====================================================/// 
/// objファイルの読み込み
///=====================================================///
void Model::LoadObjFile(const std::string& directoryPath, const std::string& filename) {

	///-------------------------------------------/// 
	/// ローカル変数
	///-------------------------------------------///

	//三角面の頂点データ
	VertexData triangle[3];

	//位置
	std::vector<Vector4> positions;

	//法線
	std::vector<Vector3> normals;

	//テクスチャ座標
	std::vector<Vector2> texcoords;

	//ファイルから読んだ1行を格納するもの
	std::string line;

	///-------------------------------------------/// 
	/// objファイルからデータを読み込む
	///-------------------------------------------///

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

				modelData_.indexes.push_back(uint32_t(modelData_.vertices.size()));
				modelData_.vertices.push_back(triangle[i]);
			}

		} else if (identifier == "mtllib") {

			/// ===  識別子が「mtllib」だったら=== ///

			std::string materialFilename;

			//materialTemplateLibraryファイルの名前を取得する
			s >> materialFilename;

			//マテリアルデータを読み込む
			modelData_.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}
}

///=====================================================/// 
/// マテリアルデータの読み込み
///=====================================================///
Model::MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {

	//構築するMaterialData
	MaterialData materialData;

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
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	return materialData;
}
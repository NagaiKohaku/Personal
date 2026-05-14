#include "Model.h"

#include <Object/3D/Mesh/ModelMesh.h>
#include <Object/2D/Texture/TextureManager.h>

#include <Math/Utility/MakeMatrixMath.h>

#include <fstream>
#include <sstream>
#include <cassert>

using namespace MyEngine;

void Model::Initialize(ModelCommon* modelCommon, MeshType type, const std::string& filePath) {

	modelCommon_ = modelCommon;

	if (type == MeshType::MODEL) {

		LoadObjFile(filePath);
	} else {

		ModelPart newPart;

		newPart.mesh = CreateMesh(type);

		modelParts_.push_back(std::move(newPart));
	}

	for (auto& part : modelParts_) {

		part.mesh->Initialize(modelCommon_->GetDxCommon());

		part.material = std::make_unique<ModelMaterial>();

		part.material->Initialize(modelCommon_->GetDxCommon());

		part.texture = std::make_unique<Texture>();

		part.texture->Initialize(part.textureFilePath);

	}
}

void Model::Draw() {

	for (auto& part : modelParts_) {

		//メッシュの設定
		part.mesh->Draw();

		//マテリアルデータの設定
		part.material->Draw(modelCommon_->GetDxCommon());

		//テクスチャデータの設定
		part.texture->Draw(modelCommon_->GetDxCommon());

		//描画コマンド発行
		modelCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(static_cast<UINT>(part.mesh->GetIndexData().size()), 1, 0, 0, 0);
	}
}

void Model::LoadObjFile(const std::string& fileName) {

	/// === ローカル変数 === ///

	//位置
	std::vector<Vector4> positions;

	//法線
	std::vector<Vector3> normals;

	//テクスチャ座標
	std::vector<Vector2> texcoords;

	//頂点データ
	std::vector<MeshBase::VertexData> vertexData;

	//頂点番号データ
	std::vector<uint32_t> indexData;

	//ファイルから読んだ1行を格納するもの
	std::string line;

	// マテリアルごとへのテクスチャマッピング
	std::unordered_map<std::string, std::string> materialMap;

	// 現在処理中のパーツインデックス
	size_t currentPartIndex = static_cast<size_t>(-1);

	/// === objファイルからデータを読み込む === ///

	modelDirectoryPath_ = kResourceDirectoryPath_ + fileName + "/";

	//ファイルを開く
	std::ifstream file(modelDirectoryPath_ + fileName + ".obj");

	//ファイルが開けたかの確認
	assert(file.is_open());

	while (std::getline(file, line)) {

		//識別子
		std::string identifier;

		//1行
		std::istringstream stream(line);

		//先頭の識別子を読む
		stream >> identifier;

		if (identifier == "mtllib") {

			/// === マテリアルファイルの読み込み === ///

			std::string materialFilename;

			//materialTemplateLibraryファイルの名前を取得する
			stream >> materialFilename;

			//マテリアルデータを読み込む
			materialMap = LoadMaterialTemplateFile(materialFilename);

		} else if (identifier == "o") {

			/// === メッシュの生成 === ///

			ModelPart newPart;

			newPart.mesh = std::make_unique<ModelMesh>();

			modelParts_.push_back(std::move(newPart));

			currentPartIndex++;

		} else if (identifier == "v") {

			//座標データを登録する
			positions.push_back(LoadVertexPosition(stream));

		} else if (identifier == "vt") {

			//テクスチャ座標データを登録
			texcoords.push_back(LoadTexCoord(stream));

		} else if (identifier == "vn") {

			//法線データを登録
			normals.push_back(LoadNormal(stream));

		} else if (identifier == "f") {

			/// === 三角面データを読み込み === ///

			//頂点データの読み込み
			std::vector<MeshBase::VertexData> triangle = LoadTriangleFace(
				stream,
				positions,
				texcoords,
				normals
			);

			//頂点データの設定
			for (int i = 2; i >= 0; i--) {

				modelParts_[currentPartIndex].mesh->AddVertexData(triangle[i]);
			}

		} else if (identifier == "usemtl") {

			/// === 使用マテリアル名の読み込み === ///

			// マテリアル名を取得
			std::string materialName;
			stream >> materialName;

			modelParts_[currentPartIndex].materialName = materialName;

			if (materialMap.contains(materialName)) {

				std::string texturePath = materialMap.at(materialName);

				modelParts_[currentPartIndex].textureFilePath = texturePath;
			}
		}
	}
}

Vector4 MyEngine::Model::LoadVertexPosition(std::istringstream& stream) {

	//頂点座標データ
	Vector4 position;

	//頂点座標データを読み込む
	stream >> position.x >> position.y >> position.z;

	//左手座標系に直す
	position.x *= -1.0f;
	position.w = 1.0f;

	return position;
}

Vector2 MyEngine::Model::LoadTexCoord(std::istringstream& stream) {

	//テクスチャ座標データ
	Vector2 texCoord;

	//テクスチャ座標データを読み込む
	stream >> texCoord.x >> texCoord.y;

	//左上が0になるように直す
	texCoord.y = 1.0f - texCoord.y;

	return texCoord;
}

Vector3 MyEngine::Model::LoadNormal(std::istringstream& stream) {

	//法線データ
	Vector3 normal;

	//法線データを読み込む
	stream >> normal.x >> normal.y >> normal.z;

	//左手座標系に直す
	normal.x *= -1.0f;

	return normal;
}

std::vector<MeshBase::VertexData> MyEngine::Model::LoadTriangleFace(
	std::istringstream& stream,
	const std::vector<Vector4>& positions,
	const std::vector<Vector2>& texcoords,
	const std::vector<Vector3>& normals
) {

	std::vector<MeshBase::VertexData> triangle;

	//面は三角形限定。その他は未対応
	for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {

		//頂点の要素
		std::string vertexDefinition;

		//頂点の要素を読み込む
		stream >> vertexDefinition;

		//頂点の要素へのIndexは[位置/UV/法線]で格納されているので、分類してIndexを取得する
		std::istringstream vertexStream(vertexDefinition);

		//頂点要素の格納用
		uint32_t elementIndices[3];

		for (int32_t element = 0; element < 3; ++element) {

			//要素
			std::string index;

			//区切りで要素を読み込む
			std::getline(vertexStream, index, '/');

			//要素を格納する
			elementIndices[element] = std::stoi(index);
		}

		//格納した要素から値を取り出していく
		Vector4 position = positions[elementIndices[0] - 1];
		Vector2 texCoord = texcoords[elementIndices[1] - 1];
		Vector3 normal = normals[elementIndices[2] - 1];

		//三角形の構築
		triangle.push_back({ position,texCoord,normal });
	}

	return triangle;
}

std::unordered_map<std::string, std::string> MyEngine::Model::LoadMaterialTemplateFile(const std::string& fileName) {

	std::unordered_map<std::string, std::string> materialMap;
	std::string currentMaterialName;

	//ファイルから読んだ1行を格納するもの
	std::string line;

	//ファイルを開く
	std::ifstream file(modelDirectoryPath_ + fileName);

	//開けなかったら止める
	assert(file.is_open());

	while (std::getline(file, line)) {

		//識別子
		std::string identifier;

		//1行
		std::istringstream stream(line);

		//先頭から識別子を読み込む
		stream >> identifier;

		if (identifier == "newmtl") {

			/// === マテリアル名の読み込み === ///

			stream >> currentMaterialName;

		} else if (identifier == "map_Kd") {

			/// === テクスチャファイル名の読み込み === ///

			//ファイル名
			std::string textureFilename;

			//ファイル名を読み込む
			stream >> textureFilename;

			//連結してファイルマップに登録する
			if (!currentMaterialName.empty()) {
				materialMap[currentMaterialName] = modelDirectoryPath_ + textureFilename;
			}
		}
	}

	return materialMap;
}
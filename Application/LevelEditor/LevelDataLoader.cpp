#include "LevelDataLoader.h"

#include "3d/Model/ModelManager.h"

#include "json.hpp"
#include "fstream"

///=====================================================/// 
/// シングルトンインスタンスの取得
///=====================================================///
LevelDataLoader* LevelDataLoader::GetInstance() {
	static LevelDataLoader instance;
	return &instance;
}

///=====================================================/// 
/// データの読み込み
///=====================================================///
void LevelDataLoader::Load(const std::string& fileName) {

	//ファイルストリーム
	std::ifstream file;

	//ファイルパス
	std::string filePath = directory_ + fileName;

	//ファイルを開く
	file.open(filePath);

	//ファイルが開けなかった場合のエラーチェック
	if (file.fail()) {
		assert(0);
	}

	//JSONデータ
	nlohmann::json deserialized;

	//ファイルからJSONデータを読み込む
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();

	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	std::vector<ObjectData> objectDataList;

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {

		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		if (object.contains("disable")) {

			//無効フラグを取得
			bool disabled = object["disable"].get<bool>();

			if (disabled) {

				continue;
			}
		}

		//種別に応じて処理を分岐
		if (type.compare("MESH") == 0) {

			/// === MESHの場合 === ///

			ObjectData newObject;

			std::string objectType;

			if (object.contains("object_group")) {

				std::string typeName;

				//オブジェクトの種別を取得
				objectType = object["object_group"].get<std::string>();

				typeName = objectType;

				std::transform(
					objectType.begin(),
					objectType.end(),
					typeName.begin(),
					[](char c) {return std::toupper(c); }
				);

				//オブジェクトの種別に応じて処理を分岐
				if (typeName == "PLAYER") {

					newObject.type = ObjectType::PLAYER;
				} else if (typeName == "ENEMY") {

					newObject.type = ObjectType::ENEMY;
				} else if (typeName == "OBJECT") {

					newObject.type = ObjectType::OBJECT;
				} else {

					newObject.type = ObjectType::NONE;
				}

			} else {

				//オブジェクトの種別が指定されていない場合はNONEに設定
				newObject.type = ObjectType::NONE;
			}

			//モデルのファイル名が存在するかチェック
			if (object.contains("file_name")) {

				//モデルのファイル名を取得
				newObject.filename = object["file_name"].get<std::string>();

				if (newObject.type != NONE) {

					//モデルの読み込み
					ModelManager::GetInstance()->LoadModel(newObject.filename, objectType, newObject.filename);
				} else {

					ModelManager::GetInstance()->LoadModel(newObject.filename, newObject.filename);
				}

			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			//平行移動
			newObject.position = {
				static_cast<float>(transform["translation"][0]),
				static_cast<float>(transform["translation"][2]),
				static_cast<float>(transform["translation"][1])
			};

			//回転
			newObject.rotation = {
				static_cast<float>(transform["rotation"][0]),
				static_cast<float>(transform["rotation"][2]),
				static_cast<float>(transform["rotation"][1])
			};

			//拡大縮小
			newObject.scale = {
				static_cast<float>(transform["scaling"][0]),
				static_cast<float>(transform["scaling"][2]),
				static_cast<float>(transform["scaling"][1])
			};

			//オブジェクトをコンテナに追加
			objectDataList.push_back(newObject);
		}
		else if (type.compare("PlayerSpawn") == 0) {

			/// === PlayerSpawnの場合 === ///

			PlayerSpawnData newData;

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			newData.position = {
				static_cast<float>(transform["translation"][0]),
				static_cast<float>(transform["translation"][2]),
				static_cast<float>(transform["translation"][1])
			};

			newData.rotation = {
				static_cast<float>(transform["rotation"][0]),
				static_cast<float>(transform["rotation"][2]),
				static_cast<float>(transform["rotation"][1])
			};
		}
	}

	//レベルデータにオブジェクトデータを追加
	levelData_.insert(std::make_pair(fileName, objectDataList));
}

///=====================================================/// 
/// オブジェクトの数を取得
///=====================================================///
const int LevelDataLoader::GetObjectCount(const std::string& fileName, const ObjectType type) const {

	//ファイル名が存在しない場合は0を返す
	if (levelData_.count(fileName) == 0) {

		return 0;
	}

	int count = 0;

	//指定されたファイル名のレベルデータ内のオブジェクトを走査
	for (const auto& object : levelData_.at(fileName)) {

		if (object.type == type) {

			//オブジェクトのタイプが一致した場合はカウントを増やす
			count++;
		}
	}

	return count;
}

///=====================================================/// 
/// 指定したタイプのオブジェクトデータを取得
///=====================================================///
const std::vector<ObjectData>& LevelDataLoader::PickObjectData(const std::string& fileName, const ObjectType type) const {

	//ファイル名が存在しない場合は空のデータを返す
	if (levelData_.count(fileName) == 0) {

		static std::vector<ObjectData> emptyData;

		return emptyData;
	}

	static std::vector<ObjectData> pickedData;

	pickedData.clear();

	//指定されたファイル名のレベルデータ内のオブジェクトを走査
	for (const auto& object : levelData_.at(fileName)) {

		if (object.type == type) {

			//オブジェクトのタイプが一致した場合はデータを追加
			pickedData.push_back(object);
		}
	}

	return pickedData;
}
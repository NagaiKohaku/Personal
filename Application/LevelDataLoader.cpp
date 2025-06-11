#include "LevelDataLoader.h"

#include "3d/Model/ModelManager.h"

#include "json.hpp"
#include "fstream"

LevelDataLoader* LevelDataLoader::GetInstance() {
	static LevelDataLoader instance;
	return &instance;
}

void LevelDataLoader::Load(const std::string& fileName) {

	//ファイルストリーム
	std::ifstream file;

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

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {

		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種別に応じて処理を分岐
		if (type.compare("MESH") == 0) {

			/// === MESHの場合 === ///

			Object newObject;

			//モデルのファイル名が存在するかチェック
			if (object.contains("file_name")) {

				//モデルのファイル名を取得
				newObject.filename = object["file_name"].get<std::string>();

				//モデルの読み込み
				ModelManager::GetInstance()->LoadModel(newObject.filename, newObject.filename);
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];

			//平行移動
			newObject.position = {
				static_cast<float>(transform["translation"][0]),
				static_cast<float>(transform["translation"][2]),
				-static_cast<float>(transform["translation"][1])
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
			objects_.push_back(newObject);
		}
	}
}

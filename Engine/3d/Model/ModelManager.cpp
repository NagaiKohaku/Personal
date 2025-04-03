#include "ModelManager.h"

#include "3d/Model/ModelCommon.h"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
ModelManager* ModelManager::GetInstance() {
	static ModelManager instance;
	return &instance;
}

///=====================================================/// 
/// 初期化処理
///=====================================================///
void ModelManager::Initialize() {

	//モデル基底のインスタンスを取得
	modelCommon_ = ModelCommon::GetInstance();

	//球体モデルの読み込み
	LoadModel("Sphere", "sphere");

	//正方形モデルの読み込み
	LoadModel("Cube", "cube");
}

///=====================================================/// 
/// モデルの読み込み
///=====================================================///
void ModelManager::LoadModel(const std::string& modelName, const std::string& modelFileName) {

	//読み込み済みモデルの検索
	if (models_.contains(modelName)) {

		//読み込み済みなら早期return
		return;
	}

	//モデルデータ
	std::unique_ptr<Model> model = std::make_unique<Model>();

	//モデルの読み込み
	model->Initialize("Resource/Model/" + modelFileName, modelFileName + ".obj");

	//モデル名とモデルデータをコンテナに登録
	models_.insert(std::make_pair(modelName, std::move(model)));
}

///=====================================================/// 
/// モデルの検索
///=====================================================///
Model* ModelManager::FindModel(const std::string& modelName) {

	//読み込み済みモデルの検索
	if (models_.contains(modelName)) {

		//読み込みモデルを戻り値としてreturn
		return models_.at(modelName).get();
	}

	//ファイル名一致なし
	return nullptr;
}
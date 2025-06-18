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



void ModelManager::CreatePrimitiveModel(const std::string& modelName, PrimitiveType type, const std::string& textureFilePath) {

	//読み込み済みモデルの検索
	if (models_.contains(modelName)) {
		//読み込み済みなら早期return
		return;
	}

	//プリミティブモデルの生成
	std::unique_ptr<Model> model = std::make_unique<Model>();

	//プリミティブモデルの初期化
	model->Initialize(type, textureFilePath);

	//モデル名とモデルデータをコンテナに登録
	models_.insert(std::make_pair(modelName, std::move(model)));
}

///=====================================================/// 
/// モデルの検索
///=====================================================///
std::unique_ptr<Model> ModelManager::FindModel(const std::string& modelName) {

	//読み込み済みモデルの検索
	if (models_.contains(modelName)) {

		Model* model = models_.at(modelName).get();

		PrimitiveType primitiveType = GetPrimitiveType(model);

		std::unique_ptr<Model> newModel = std::make_unique<Model>();

		newModel->Copy(model);

		newModel->Initialize(primitiveType);

		//読み込みモデルを戻り値としてreturn
		return std::move(newModel);
	}

	//ファイル名一致なし
	return nullptr;
}
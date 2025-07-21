#include "ModelManager.h"

#include "3d/Model/ModelCommon.h"

///=====================================================/// 
/// シングルトンインスタンスを取得
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

	/// === 初期モデルの生成 === ///

	//PlaneMeshモデルの生成
	CreateMeshModel("Plane", MeshType::PLANE, "Resource/Texture/white_128x128.png");

	//RingMeshモデルの生成
	CreateMeshModel("Ring", MeshType::RING, "Resource/Texture/white_128x128.png");

	//CylinderMeshモデルの生成
	CreateMeshModel("Cylinder", MeshType::CYLINDER, "Resource/Texture/white_128x128.png");

	//Sphereモデルの生成
	CreateMeshModel("Sphere", MeshType::SPHERE, "Resource/Texture/white_128x128.png");

	//Cubeモデルの生成
	CreateMeshModel("Cube", MeshType::CUBE, "Resource/Texture/white_128x128.png");

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

void ModelManager::LoadModel(const std::string& modelName, const std::string& modelDirectory, const std::string& modelFileName) {

	//読み込み済みモデルの検索
	if (models_.contains(modelName)) {

		//読み込み済みなら早期return
		return;
	}

	//モデルデータ
	std::unique_ptr<Model> model = std::make_unique<Model>();

	//モデルの読み込み
	model->Initialize("Resource/Model/" + modelDirectory, modelFileName + ".obj");

	//モデル名とモデルデータをコンテナに登録
	models_.insert(std::make_pair(modelName, std::move(model)));
}

///=====================================================/// 
/// メッシュモデルの生成
///=====================================================///
void ModelManager::CreateMeshModel(const std::string& modelName, MeshType type, const std::string& textureFilePath) {

	//読み込み済みモデルの検索
	if (models_.contains(modelName)) {
		//読み込み済みなら早期return
		return;
	}

	//メッシュモデルの生成
	std::unique_ptr<Model> model = std::make_unique<Model>();

	//メッシュモデルの初期化
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

		//リストからモデルを取得
		Model* model = models_.at(modelName).get();

		//メッシュタイプを取得
		MeshType meshType = GetMeshType(model);

		//モデルを生成
		std::unique_ptr<Model> newModel = std::make_unique<Model>();

		//初期化
		newModel->Initialize(meshType, model);

		//読み込みモデルを戻り値としてreturn
		return std::move(newModel);
	}

	//ファイル名一致なし
	return nullptr;
}
#include "ModelManager.h"

namespace MyEngine {

	///=====================================================/// 
	/// ModelManagerのシングルトンインスタンスを取得
	///=====================================================///
	ModelManager* ModelManager::GetInstance() {
		static ModelManager instance;
		return &instance;
	}

	///=====================================================/// 
	/// モデルマネージャを初期化し、基本的なプリミティブモデルを生成
	///=====================================================///
	void ModelManager::Initialize(ModelCommon* modelCommonPtr) {

		//モデル基底のインスタンスを取得
		modelCommon_ = modelCommonPtr;

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

	///=============================================================/// 
	/// 指定されたモデル名とファイル名からモデルを読み込み、管理コンテナに登録
	///=============================================================///
	void ModelManager::LoadModel(const std::string& modelName, const std::string& modelFileName) {

		//読み込み済みモデルの検索
		if (models_.contains(modelName)) {

			//読み込み済みなら早期return
			return;
		}

		//モデルデータ
		std::unique_ptr<Model> model = std::make_unique<Model>();

		//モデルの読み込み
		model->Initialize(modelCommon_, MeshType::MODEL, modelFileName);

		//モデル名とモデルデータをコンテナに登録
		models_.insert(std::make_pair(modelName, std::move(model)));
	}

	///==================================================================/// 
	/// 指定したタイプのメッシュモデルを生成し、テクスチャを設定して管理コンテナに登録
	///==================================================================///
	void ModelManager::CreateMeshModel(const std::string& modelName, MeshType type, const std::string& textureFilePath) {

		//読み込み済みモデルの検索
		if (models_.contains(modelName)) {
			//読み込み済みなら早期return
			return;
		}

		//メッシュモデルの生成
		std::unique_ptr<Model> model = std::make_unique<Model>();

		//メッシュモデルの初期化
		model->Initialize(modelCommon_, type, textureFilePath);

		//モデル名とモデルデータをコンテナに登録
		models_.insert(std::make_pair(modelName, std::move(model)));
	}

	///=====================================================/// 
	/// 登録済みモデルを検索
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
}
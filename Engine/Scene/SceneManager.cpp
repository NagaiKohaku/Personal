#include "SceneManager.h"

/// ===シーン=== ///
#include <Scene/BaseScene.h>

#include <3d/Collider/ColliderManager.h>
#include <Scene/EngineContext.h>

#include <Scene/GameScene/GameScene.h>
#include <Scene/TitleScene.h>
#include <Scene/ParticleEditorScene.h>

namespace MyEngine {

	///=====================================================/// 
	/// SceneManagerのシングルトンインスタンスを取得
	///=====================================================///
	SceneManager* SceneManager::GetInstance() {
		static SceneManager instance;
		return &instance;
	}

	///=====================================================/// 
	/// 現在設定されているシーンの初期化処理を実行
	///=====================================================///
	void SceneManager::Initialize(EngineContext context) {

		context_ = context;

		/// ===シーンの中身の確認=== ///

		if (currentScene_) {

			currentScene_->Initialize(context_);
		}
	}

	///=====================================================/// 
	/// 現在設定されているシーンの更新処理を実行
	///=====================================================///
	void SceneManager::Update() {

		/// ===シーンの中身の確認=== ///

		if (currentScene_) {

			//現在のシーンの更新
			currentScene_->Update();
		}
	}

	///=====================================================/// 
	/// 現在設定されているシーンの描画処理を実行
	///=====================================================///
	void SceneManager::Draw() {

		/// ===シーンの中身の確認=== ///

		if (currentScene_) {

			//現在シーンの描画
			currentScene_->Draw();
		}
	}

	///=====================================================/// 
	/// 現在設定されているシーンのImGui処理を実行
	///=====================================================///
	void SceneManager::ImGui() {

		/// ===シーンの中身の確認=== ///

		if (currentScene_) {

			//現在のシーンのデバッグ
			currentScene_->ImGui();
		}
	}

	///=====================================================/// 
	/// 現在のシーンを指定された種類の新しいシーンに切り替える
	///=====================================================///
	void SceneManager::ChangeScene(SceneType sceneType) {

		//コライダーをすべて破棄
		MyEngine::ColliderManager::GetInstance()->ClearColliders();

		//シーンがすでにある場合は終了処理を実行
		if (currentScene_) {

			MyEngine::BaseScene* preScene = currentScene_.get();

			preScene->Finalize();
		}

		// 新しいシーンを生成
		currentScene_ = CreateScene(sceneType);

		// シーンの初期化
		Initialize(context_);
	}

	///=====================================================/// 
	/// 指定された種類に応じたシーンオブジェクトを生成して返す
	///=====================================================///
	std::unique_ptr<MyEngine::BaseScene> SceneManager::CreateScene(SceneType sceneType) {

		/// ===シーンの判断=== ///

		switch (sceneType) {

			//タイトルシーン
		case SceneType::kTitle:

			return std::make_unique<TitleScene>();

			//ゲームシーン
		case SceneType::kGame:

			return std::make_unique<GameScene>();

		case SceneType::kParticleEditor:

			//パーティクルエディターシーン
			return std::make_unique<ParticleEditorScene>();

		default:
			return nullptr;
		}
	}
}
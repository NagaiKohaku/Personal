#include "SceneManager.h"

/// ===シーン=== ///
#include "BaseScene.h"

#include "Scene/GameScene.h"
#include "Scene/ParticleEditorScene.h"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

///=====================================================/// 
/// 初期化
///=====================================================///
void SceneManager::Initialize() {

	/// ===シーンの中身の確認=== ///

	if (currentScene_) {

		currentScene_->Initialize();
	}
}

///=====================================================/// 
/// 更新処理
///=====================================================///
void SceneManager::Update() {

	/// ===シーンの中身の確認=== ///

	if (currentScene_) {

		//現在のシーンの更新
		currentScene_->Update();
	}
}

///=====================================================/// 
/// 描画処理
///=====================================================///
void SceneManager::Draw() {

	/// ===シーンの中身の確認=== ///

	if (currentScene_) {

		//現在シーンの描画
		currentScene_->Draw();
	}
}

///=====================================================/// 
/// ImGui処理
///=====================================================///
void SceneManager::ImGui() {

	/// ===シーンの中身の確認=== ///

	if (currentScene_) {

		//現在のシーンのデバッグ
		currentScene_->ImGui();
	}
}

///=====================================================/// 
/// シーンの変更
///=====================================================///
void SceneManager::ChangeScene(SceneType sceneType) {

	// 新しいシーンを生成
	currentScene_ = CreateScene(sceneType);
	
	// シーンの初期化
	Initialize();
}

///=====================================================/// 
/// シーンの生成
///=====================================================///
std::unique_ptr<BaseScene> SceneManager::CreateScene(SceneType sceneType) {

	/// ===シーンの判断=== ///

	switch (sceneType) {

		//タイトルシーン
	case SceneType::kTitle:

		//return std::make_unique<TitleScene>();

		//ゲームシーン
	case SceneType::kGame:

		return std::make_unique<GameScene>();

	case SceneType::kParticleEditor:

		return std::make_unique<ParticleEditorScene>();

	default:
		return nullptr;
	}
}
#include "SceneManager.h"

/// ===シーン=== ///
#include "BaseScene.h"

#include "GameScene.h"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

///-------------------------------------------/// 
///初期化
///-------------------------------------------///
void SceneManager::Initialize() {
	/// ===シーンの中身の確認=== ///
	if (currentScene_) {
		currentScene_->Initialize();
	}
}

///-------------------------------------------/// 
///更新処理
///-------------------------------------------///
void SceneManager::Update() {
	/// ===シーンの中身の確認=== ///
	if (currentScene_) {
		//現在のシーンの更新
		currentScene_->Update();
	}

	/// ===シーン管理ImGui=== ///
	SceneObserver();
}

///-------------------------------------------/// 
///描画処理
///-------------------------------------------///
void SceneManager::Draw() {
	/// ===シーンの中身の確認=== ///
	if (currentScene_) {
		//現在シーンの描画
		currentScene_->Draw();
	}
}

///=====================================================/// 
///シーンの管理
///=====================================================///
///-------------------------------------------/// 
///シーン変更
///-------------------------------------------///
void SceneManager::ChangeScene(SceneType sceneType) {
	// 新しいシーンを生成
	currentScene_ = CreateScene(sceneType);
	// 新しいシーンに SceneManager をセット
	if (currentScene_) {
		currentScene_->SetSceneManager(this);
	}
	// シーンの初期化
	Initialize();
}

///-------------------------------------------/// 
///シーンの生成
///-------------------------------------------///
std::unique_ptr<BaseScene> SceneManager::CreateScene(SceneType sceneType) {
	/// ===シーンの判断=== ///
	switch (sceneType) {
		//タイトルシーン
	case SceneType::kTitle:
		//return std::make_unique<TitleScene>();

		//ゲームシーン
	case SceneType::kGame:
		return std::make_unique<GameScene>();
		// 他のシーンタイプの処理
	default:
		return nullptr;
	}
}

///-------------------------------------------/// 
///シーン監視
///-------------------------------------------///
void SceneManager::SceneObserver() {
	//ImGui::Begin("Scene Manager");
	//
	///// ===現在のシーンを表示=== /// 
	//if (currentScene_) {
	//	std::string currentSceneName = typeid( *currentScene_ ).name();
	//	ImGui::Text("Current Scene: %s", currentSceneName.c_str());
	//} else {
	//	ImGui::Text("No Current Scene");
	//}
	//
	///// ===シーン変更用のドロップダウンメニュー=== ///
	//if (ImGui::BeginCombo("Select Scene", "Change Scene")) {
	//
	//	///タイトルシーン
	//	if (ImGui::Selectable("Title Scene", currentScene_ && dynamic_cast<TitleScene*>( currentScene_.get() ))) {
	//		ChangeScene(SceneType::kTitle);
	//	}
	//	///ゲームシーン
	//	if (ImGui::Selectable("Game Scene", currentScene_ && dynamic_cast<GameScene*>( currentScene_.get() ))) {
	//		ChangeScene(SceneType::kGame);
	//	}
	//	ImGui::EndCombo();
	//}
	//ImGui::End();
}

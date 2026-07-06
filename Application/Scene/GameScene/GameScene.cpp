#include "GameScene.h"

#include "Base/OffScreen.h"
#include "Base/Input.h"
#include "Scene/SceneManager.h"
#include <Object/Manager/ObjectManager.h>
#include <Effect/Manager/UIManager.h>
#include <Effect/Manager/EmitterManager.h>

#include "Scene/GameScene/Event/StartEvent.h"
#include "Scene/GameScene/Event/GameEvent.h"
#include "Scene/GameScene/Event/PauseEvent.h"
#include "Scene/GameScene/Event/ClearEvent.h"
#include "Scene/GameScene/Event/GameOverEvent.h"

#include "2d/Sprite/SpriteManager.h"
#include "3d/Model/ModelManager.h"
#include "2d/Sprite/TextureManager.h"

#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"
#include "3d/Object/SkyBoxCommon.h"

#include "LevelEditor/LevelDataLoader.h"
#include <Effect/Fade/Fade.h>
#include <Effect/Shake/Shake.h>
#include <Effect/Flash/Flash.h>

#include <Math/Utility/Easing.h>
#include <Math/Utility/MakeMatrixMath.h>

#include "imgui.h"

#include "numbers"

#include <Scene/EngineContext.h>

using namespace MyEngine;

///=====================================================/// 
/// 初期化
///=====================================================///
void GameScene::Initialize(MyEngine::EngineContext context) {

	//シーンの初期化
	BaseScene::Initialize(context);

	/// === カメラの設定 === ///

	//デバッグカメラを使用しない
	context_.camera->SetDebugCameraFlag(false);

	//シェイクにカメラをセット
	Shake::GetInstance()->SetCamera(context_.camera);

	//エミッターマネージャーにカメラをセット
	EmitterManager::GetInstance()->SetCamera(context_.camera);

	/// === エネミーマネージャーの生成 === ///

	enemyManager_ = std::make_unique<EnemyManager>();

	/// === バレットマネージャーの生成 === ///

	bulletManager_ = std::make_unique<BulletManager>();

	bulletManager_->Initialize();

	/// === グラウンドマネージャーの生成 === ///

	groundManager_ = GroundManager::GetInstance();

	groundManager_->Initialize();

	/// === プレイヤーの生成 === ///

	ObjectManager::GetInstance()->SpawnPlayer();

	player_ = ObjectManager::GetInstance()->GetPlayer();

	player_->Initialize(context_.camera, bulletManager_.get(), false);

	/// === 敵の生成 === ///

	enemyManager_->Initialize(context_.camera, bulletManager_.get(), player_);

	/// === 追尾カメラの生成 === ///

	followCamera_ = std::make_unique<FollowCamera>();

	followCamera_->Initialize(context_.camera, player_);

	//最初は追従を無効化する
	followCamera_->SetIsActive(false);

	/// === UIの生成 === ///

	//ゲームシーンのUIを生成する
	UIManager::GetInstance()->LoadUI("GameScene");

	/// === イベントの生成 === ///

	sceneProgress_ = std::make_unique<GameSceneProgress>();

	sceneProgress_->Initialize(context_, player_, followCamera_.get());

	/// === フェードの設定 === ///

	Fade::GetInstance()->SetPlayer(player_);

	//フェードイン開始
	Fade::GetInstance()->StartFadeIn();

}

///=====================================================/// 
/// 終了処理
///=====================================================///
void GameScene::Finalize() {

	ObjectManager::GetInstance()->ClearAll();

	UIManager::GetInstance()->DeleteAllUI();

	Fade::GetInstance()->SetPlayer(nullptr);

	Shake::GetInstance()->SetCamera(nullptr);

	//音声データの解放
	Audio::GetInstance()->Finalize();
}

///=====================================================/// 
/// 更新
///=====================================================///
void GameScene::Update() {

	if (Fade::GetInstance()->GetState() == Fade::FadeState::FADE_IN_END) {
		Fade::GetInstance()->SetState(Fade::FadeState::NONE);
	}

	//追尾カメラの更新
	followCamera_->Update();

	//カメラをデバッグ状態で更新
	context_.camera->Update();

	sceneProgress_->Update();

	if (sceneProgress_->canMove()) {

		//プレイヤーの更新
		player_->Update();

		//エネミーの更新
		enemyManager_->Update();

		//弾の更新
		bulletManager_->Update();

		//グラウンドマネージャーの更新
		groundManager_->Update();
	} else {

		player_->TransformUpdate();

		enemyManager_->TransformUpdate();

		bulletManager_->TransformUpdate();

		groundManager_->TransformUpdate();
	}

	//フェードアウトが終わったら
	if (Fade::GetInstance()->GetState() == Fade::FadeState::FADE_OUT_END) {

		//フェードの状態をリセット
		Fade::GetInstance()->SetState(Fade::FadeState::NONE);

		//タイトルシーンに遷移
		SceneManager::GetInstance()->ChangeScene(SceneManager::SceneType::kTitle);
	}
}

///=====================================================/// 
/// 描画
///=====================================================///
void GameScene::Draw() {

	//プレイヤーの描画
	player_->Draw();

	//エネミーの描画
	enemyManager_->Draw();

	//弾の描画
	bulletManager_->Draw();

	//グラウンドマネージャーの描画
	groundManager_->Draw();

	//イベントの描画
	sceneProgress_->Draw();

}

///=====================================================/// 
/// ImGui
///=====================================================///
void GameScene::ImGui() {

#ifdef _USE_IMGUI

	//ImGuiを起動
	ImGui::Begin("Scene");

	if (ImGui::TreeNode("Camera")) {

		context_.camera->DisplayImGui();

		ImGui::TreePop();
	}

	ImGui::Text("Shift + LeftClick : Move Camera");
	ImGui::Text("Shift + RightClick : Rotate Camera");
	ImGui::Text("Shift + MiddleWheel : Move Offset Camera");

	groundManager_->ImGui();

	bulletManager_->ImGui();

	enemyManager_->ImGui();

	//ImGuiの終了
	ImGui::End();

#endif // _USE_IMGUI

}

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

using namespace MyEngine;

///=====================================================/// 
/// 初期化
///=====================================================///
void GameScene::Initialize() {

	/// === カメラの設定 === ///

	camera_ = std::make_unique<Camera>();

	camera_->Initialize();

	//デバッグカメラを使用しない
	camera_->SetDebugCameraFlag(false);

	//シェイクにカメラをセット
	Shake::GetInstance()->SetCamera(camera_.get());

	EmitterManager::GetInstance()->SetCamera(camera_.get());

	/// === エネミーマネージャーの生成 === ///

	enemyManager_ = std::make_unique<EnemyManager>();

	/// === バレットマネージャーの生成 === ///

	bulletManager_ = std::make_unique<BulletManager>();

	bulletManager_->Initialize();

	/// === グラウンドマネージャーの生成 === ///

	groundManager_ = std::make_unique<GroundManager>();

	groundManager_->Initialize();

	/// === プレイヤーの生成 === ///

	ObjectManager::GetInstance()->SpawnPlayer();

	player_ = ObjectManager::GetInstance()->GetPlayer();

	player_->Initialize(camera_.get(), bulletManager_.get(), false);

	//エネミーマネージャーの初期化
	enemyManager_->Initialize(camera_.get(), bulletManager_.get(), player_);

	/// === 追尾カメラの生成 === ///

	followCamera_ = std::make_unique<FollowCamera>();

	followCamera_->Initialize(camera_.get(), player_);

	//最初は無効化する
	followCamera_->SetIsActive(false);

	/// === ゲームオーバースプライトの生成 === ///

	UIManager::GetInstance()->LoadUI("GameScene");

	spaceKeyPos_ = UIManager::GetInstance()->Get2DObject("GameOver", "Space")->GetTranslate();

	spaceKeySize_ = UIManager::GetInstance()->Get2DObject("GameOver", "Space")->GetSize();

	/// === エミッターの生成 === ///

	//衝撃波エミッター(左)
	shockWaveLeftEmitter_ = std::make_unique<EmitterGroup>();

	shockWaveLeftEmitter_->Initialize(camera_.get());

	shockWaveLeftEmitter_->LoadEmitter("ShockWaveLeft");

	//衝撃波エミッター(右)

	shockWaveRightEmitter_ = std::make_unique<EmitterGroup>();

	shockWaveRightEmitter_->Initialize(camera_.get());

	shockWaveRightEmitter_->LoadEmitter("ShockWaveRight");

	sceneProgress_ = std::make_unique<GameSceneProgress>();

	sceneProgress_->Initialize(player_,camera_.get(),followCamera_.get());

	/// === 他変数の設定 === ///

	arrowLength_ = 20.0f;

	arrowTimer_ = 0.0f;

	timerDirection_ = 1.0f;

	Fade::GetInstance()->SetCamera(camera_.get());

	Fade::GetInstance()->SetPlayer(player_);

	Fade::GetInstance()->StartFadeIn();

}

///=====================================================/// 
/// 終了処理
///=====================================================///
void GameScene::Finalize() {

	ObjectManager::GetInstance()->ClearAll();

	UIManager::GetInstance()->DeleteAllUI();

	//演出系の参照リセット
	Fade::GetInstance()->SetCamera(nullptr);

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
	camera_->Update();

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

	arrowTimer_ += (1.0f / 60.0f) * timerDirection_;

	if (arrowTimer_ >= 1.0f) {

		arrowTimer_ = 1.0f;

		timerDirection_ *= -1.0f;
	}

	if (arrowTimer_ <= 0.0f) {

		arrowTimer_ = 0.0f;

		timerDirection_ *= -1.0f;
	}

	float lerpNum = EaseOut(0.0f, arrowLength_, arrowTimer_ / 1.0f);

	float alphaNum = EaseOut(0.0f, 1.0f, arrowTimer_ / 1.0f);

	//3Dオブジェクトの座標をスクリーン座標に変換する
	Vector3 playerScreenPos = Vector3ToScreenSpace(camera_.get(), player_->GetWorldPos());

	UIManager::GetInstance()->Get2DObject("GameOver", "LeftArrow")->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f - lerpNum,spaceKeyPos_.y });

	UIManager::GetInstance()->Get2DObject("GameOver", "RightArrow")->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f + lerpNum,spaceKeyPos_.y });

	UIManager::GetInstance()->Get2DObject("Clear", "LeftArrow")->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f - lerpNum,spaceKeyPos_.y });

	UIManager::GetInstance()->Get2DObject("Clear", "RightArrow")->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f + lerpNum,spaceKeyPos_.y });

	UIManager::GetInstance()->GetUIGroup("Reticle")->transform.translate_ = playerScreenPos;

	UIManager::GetInstance()->Get2DObject("Pause", "Text")->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, alphaNum));

	if (Input::GetInstance()->isPushKey(DIK_W)) {

		UIManager::GetInstance()->Get2DObject("Reticle", "WButton")->GetSprite()->SetRatio(1.0f);
	} else {

		UIManager::GetInstance()->Get2DObject("Reticle", "WButton")->GetSprite()->SetRatio(0.0f);
	}

	if (Input::GetInstance()->isPushKey(DIK_A)) {

		UIManager::GetInstance()->Get2DObject("Reticle", "AButton")->GetSprite()->SetRatio(1.0f);
	} else {

		UIManager::GetInstance()->Get2DObject("Reticle", "AButton")->GetSprite()->SetRatio(0.0f);
	}

	if (Input::GetInstance()->isPushKey(DIK_S)) {

		UIManager::GetInstance()->Get2DObject("Reticle", "SButton")->GetSprite()->SetRatio(1.0f);
	} else {

		UIManager::GetInstance()->Get2DObject("Reticle", "SButton")->GetSprite()->SetRatio(0.0f);
	}

	if (Input::GetInstance()->isPushKey(DIK_D)) {

		UIManager::GetInstance()->Get2DObject("Reticle", "DButton")->GetSprite()->SetRatio(1.0f);
	} else {

		UIManager::GetInstance()->Get2DObject("Reticle", "DButton")->GetSprite()->SetRatio(0.0f);
	}

	UIManager::GetInstance()->Get2DObject("Reticle", "SpaceButton")->GetSprite()->SetRatio(player_->GetAttackTimeRatio());

	//右衝撃波エミッターの更新
	shockWaveRightEmitter_->Update();

	//左衝撃波エミッターの更新
	shockWaveLeftEmitter_->Update();

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

	//右衝撃波エミッターの描画
	shockWaveRightEmitter_->Draw();

	//左衝撃波エミッターの描画
	shockWaveLeftEmitter_->Draw();

}

///=====================================================/// 
/// ImGui
///=====================================================///
void GameScene::ImGui() {

#ifdef _USE_IMGUI

	//ImGuiを起動
	ImGui::Begin("Scene");

	if (ImGui::TreeNode("Camera")) {

		camera_->DisplayImGui();

		ImGui::TreePop();
	}

	ImGui::Text("Shift + LeftClick : Move Camera");
	ImGui::Text("Shift + RightClick : Rotate Camera");
	ImGui::Text("Shift + MiddleWheel : Move Offset Camera");

	//ImGuiの終了
	ImGui::End();

#endif // _USE_IMGUI

}

#include "GameScene.h"

#include "Base/OffScreen.h"
#include "Base/Input.h"
#include "Scene/SceneManager.h"

#include "2d/Sprite/SpriteManager.h"
#include "3d/Model/ModelManager.h"
#include "2d/Sprite/TextureManager.h"

#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"
#include "3d/Object/SkyBoxCommon.h"

#include "LevelEditor/LevelDataLoader.h"
#include "Fade/Fade.h"
#include "Math/Easing.h"

#include "imgui.h"

#include "numbers"

///=====================================================/// 
/// 初期化
///=====================================================///
void GameScene::Initialize() {

	/// === カメラの設定 === ///

	//カメラを生成
	camera_ = std::make_unique<Camera>();

	//カメラの初期化
	camera_->Initialize();

	//デバッグカメラを使用しない
	camera_->SetDebugCameraFlag(false);

	//カメラの座標
	camera_->GetWorldTransform().translate_ = { 0.0f,3.0f,0.0f };

	/// === オブジェクトマネージャーの生成 === ///

	//エネミーマネージャーの生成
	enemyManager_ = std::make_unique<EnemyManager>();

	//バレットマネージャーの生成
	bulletManager_ = std::make_unique<BulletManager>();

	//バレットマネージャーの初期化
	bulletManager_->Initialize();

	/// === オブジェクトの生成 === ///

	//プレイヤーの生成
	player_ = std::make_unique<Player>();

	//プレイヤーの初期化
	player_->Initialize(camera_.get(), bulletManager_.get());

	player_->SetIsMoveActive(false);

	//エネミーマネージャーの初期化
	enemyManager_->Initialize(camera_.get(), bulletManager_.get(), player_.get());

	//追尾カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();

	//追尾カメラの初期化
	followCamera_->Initialize(camera_.get(), player_.get());

	followCamera_->SetIsActive(false);

	//lineGround_ = std::make_unique<LineGround>();

	//lineGround_->Initialize();

	groundManager_ = std::make_unique<GroundManager>();

	groundManager_->Initialize();

	SpriteManager::GetInstance()->LoadSprite("ToTitle", "ToTitle");

	titleSprite_ = std::make_unique<Object2D>();

	titleSprite_->Initialize();

	titleSprite_->SetSprite("ToTitle");

	titleSprite_->SetSize({ 1280.0f,720.0f });

	titleSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	titleSprite_->SetTranslate({ 640.0f,320.0f });

	timer_ = 0.0f;

	animNum_ = 0;

	isStart_ = true;

	animPoints_.push_back({ Vector3(0.0f,20.0f,-600.0f),Vector3(0.0f,-std::numbers::pi_v<float>,0.0f),0.0f,1.0f });
	animPoints_.push_back({ Vector3(0.0f,20.0f,-600.0f),Vector3(0.0f,-std::numbers::pi_v<float>,0.0f),1.0f,1.0f });
	animPoints_.push_back({ Vector3(0.0f,2.0f,0.0f),Vector3(0.2f,-std::numbers::pi_v<float>,0.0f),2.0f,3.0f });
	animPoints_.push_back({ Vector3(0.0f,2.0f,0.0f),Vector3(0.2f,0.0f,0.0f),4.0f,4.0f });

	Fade::GetInstance()->SetCamera(camera_.get());

	Fade::GetInstance()->SetPlayer(player_.get());

	Fade::GetInstance()->StartFadeIn();

	//NOTE:地面とSkyBoxはテクスチャが見にくいためいったんコメントアウト

	//skyBox_ = std::make_unique<SkyBox>();

	//skyBox_->Initialize("raceTrackSkyBox.dds");

}

///=====================================================/// 
/// 終了処理
///=====================================================///
void GameScene::Finalize() {

	Fade::GetInstance()->SetCamera(nullptr);

	Fade::GetInstance()->SetPlayer(nullptr);

	//音声データの解放
	Audio::GetInstance()->Finalize();
}

///=====================================================/// 
/// 更新
///=====================================================///
void GameScene::Update() {

	Start();

	//追尾カメラの更新
	followCamera_->Update();

	//カメラをデバッグ状態で更新
	camera_->Update();

	//プレイヤーの更新
	player_->Update();

	//エネミーの更新
	enemyManager_->Update();

	//弾の更新
	bulletManager_->Update();

	//lineGround_->Update();

	groundManager_->Update();

	titleSprite_->Update();

	if (Input::GetInstance()->IsTriggerPushKey(DIK_T)) {

		Fade::GetInstance()->StartFadeOut();
	}

	if (Fade::GetInstance()->GetState() == Fade::FADE_OUT_END) {

		Fade::GetInstance()->SetState(Fade::NONE);

		SceneManager::GetInstance()->ChangeScene(SceneManager::kTitle);
	}

	//NOTE:地面とSkyBoxはテクスチャが見にくいためいったんコメントアウト

	//skyBox_->Update();
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

	//lineGround_->Draw();

	groundManager_->Draw();

	titleSprite_->Draw(LayerType::UI);

	//NOTE:地面とSkyBoxはテクスチャが見にくいためいったんコメントアウト

	//skyBox_->Draw();
}

///=====================================================/// 
/// ImGui
///=====================================================///
void GameScene::ImGui() {

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
}

void GameScene::Start() {

	if (!isStart_) {
		return;
	}

	timer_ += 1.0f / 60.0f;

	if (animNum_ == 0) {

		player_->SetPosition(animPoints_[animNum_].playerPos);

		camera_->GetWorldTransform().rotate_ = animPoints_[animNum_].cameraRot;

		animNum_++;
	} else {

		float t = (timer_ - animPoints_[animNum_ - 1].time) / (animPoints_[animNum_].time - animPoints_[animNum_ - 1].time);

		if (t >= 1.0f) {

			t = 1.0f;
		}

		player_->SetPosition(EaseOut(animPoints_[animNum_ - 1].playerPos, animPoints_[animNum_].playerPos, t, animPoints_[animNum_].mag));

		camera_->GetWorldTransform().rotate_ = EaseOut(animPoints_[animNum_ - 1].cameraRot, animPoints_[animNum_].cameraRot, t, animPoints_[animNum_].mag);

		if (t == 1.0f) {

			animNum_++;
		}

		if (animNum_ == static_cast<int>(animPoints_.size())) {

			isStart_ = false;

			followCamera_->SetIsActive(true);

			player_->SetIsMoveActive(true);
		}
	}

}
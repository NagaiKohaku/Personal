#include "GameScene.h"

#include "Base/OffScreen.h"
#include "Base/Input.h"
#include "Scene/SceneManager.h"
#include "Base/OffScreen.h"

#include "2d/Sprite/SpriteManager.h"
#include "3d/Model/ModelManager.h"
#include "2d/Sprite/TextureManager.h"

#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"
#include "3d/Object/SkyBoxCommon.h"

#include "LevelEditor/LevelDataLoader.h"
#include "Fade/Fade.h"
#include "Shake/Shake.h"
#include "Math/Easing.h"

#include "imgui.h"

#include "numbers"

///=====================================================/// 
/// 初期化
///=====================================================///
void GameScene::Initialize() {

	/// === カメラの設定 === ///

	camera_ = std::make_unique<Camera>();

	camera_->Initialize();

	//デバッグカメラを使用しない
	camera_->SetDebugCameraFlag(false);

	//カメラの座標
	camera_->GetWorldTransform().translate_ = { 0.0f,3.0f,0.0f };

	//シェイクにカメラをセット
	Shake::GetInstance()->SetCamera(camera_.get());

	/// === エネミーマネージャーの生成 === ///

	enemyManager_ = std::make_unique<EnemyManager>();

	/// === バレットマネージャーの生成 === ///

	bulletManager_ = std::make_unique<BulletManager>();

	bulletManager_->Initialize();

	/// === グラウンドマネージャーの生成 === ///

	groundManager_ = std::make_unique<GroundManager>();

	groundManager_->Initialize();

	/// === プレイヤーの生成 === ///

	player_ = std::make_unique<Player>();

	player_->Initialize(camera_.get(), bulletManager_.get());

	//最初は動けないようにする
	player_->SetIsMoveActive(false);

	//エネミーマネージャーの初期化
	enemyManager_->Initialize(camera_.get(), bulletManager_.get(), player_.get());

	/// === 追尾カメラの生成 === ///

	followCamera_ = std::make_unique<FollowCamera>();

	followCamera_->Initialize(camera_.get(), player_.get());

	//最初は無効化する
	followCamera_->SetIsActive(false);

	/// === スプライトの読み込み === ///

	SpriteManager::GetInstance()->LoadSprite("GameOver", "RoadFlightGameOver");

	SpriteManager::GetInstance()->LoadSprite("SpaceKey", "space");

	SpriteManager::GetInstance()->LoadSprite("Arrow", "triangleArrow");

	SpriteManager::GetInstance()->LoadSprite("ArrowFlip", "triangleArrowFlip");

	/// === ゲームオーバースプライトの生成 === ///

	gameOverSprite_ = std::make_unique<Object2D>();

	gameOverSprite_->Initialize();

	gameOverSprite_->SetSprite("GameOver");

	gameOverSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameOverSprite_->SetTranslate({ 640.0f,100.0f });

	gameOverSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	/// === スペースキースプライトの生成 === ///

	spaceKeySprite_ = std::make_unique<Object2D>();

	spaceKeySprite_->Initialize();

	spaceKeySprite_->SetSprite("SpaceKey");

	spaceKeySprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	spaceKeySprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	spaceKeySprite_->SetTranslate({ 640.0f,600.0f });

	spaceKeySprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	Vector2 spaceKeyPos = spaceKeySprite_->GetTranslate();

	Vector2 spaceKeySize = spaceKeySprite_->GetSize();

	/// === 左矢印スプライトの生成 === ///

	leftArrowSprite_ = std::make_unique<Object2D>();

	leftArrowSprite_->Initialize();

	leftArrowSprite_->SetSprite("Arrow");

	leftArrowSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	leftArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	leftArrowSprite_->SetTranslate({ spaceKeyPos.x - spaceKeySize.x / 2.0f - 64.0f,spaceKeyPos.y });

	leftArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	/// === 右矢印の生成 === ///

	rightArrowSprite_ = std::make_unique<Object2D>();

	rightArrowSprite_->Initialize();

	rightArrowSprite_->SetSprite("ArrowFlip");

	rightArrowSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	rightArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	rightArrowSprite_->SetTranslate({ spaceKeyPos.x + spaceKeySize.x / 2.0f + 64.0f,spaceKeyPos.y });

	rightArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	/// === 他変数の設定 === ///

	timer_ = 0.0f;

	animNum_ = 0;

	isStart_ = true;

	//キーフレームの設定
	animPoints_.push_back({ Vector3(0.0f,20.0f,-600.0f),Vector3(0.0f,-std::numbers::pi_v<float>,0.0f),0.0f,1.0f });
	animPoints_.push_back({ Vector3(0.0f,20.0f,-600.0f),Vector3(0.0f,-std::numbers::pi_v<float>,0.0f),1.0f,1.0f });
	animPoints_.push_back({ Vector3(0.0f,2.0f,0.0f),Vector3(0.2f,-std::numbers::pi_v<float>,0.0f),2.0f,3.0f });
	animPoints_.push_back({ Vector3(0.0f,2.0f,0.0f),Vector3(0.2f,0.0f,0.0f),4.0f,4.0f });

	Fade::GetInstance()->SetCamera(camera_.get());

	Fade::GetInstance()->SetPlayer(player_.get());

	Fade::GetInstance()->StartFadeIn();

}

///=====================================================/// 
/// 終了処理
///=====================================================///
void GameScene::Finalize() {

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

	//スタート時のアニメーションの更新
	StartAnimation();

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

	//グラウンドマネージャーの更新
	groundManager_->Update();

	//ゲームオーバースプライトの更新
	gameOverSprite_->Update();

	//スペースキースプライトの更新
	spaceKeySprite_->Update();

	//左矢印スプライトの更新
	leftArrowSprite_->Update();

	//右矢印スプライトの更新
	rightArrowSprite_->Update();

	//プレイヤーが倒されたとき
	if (!isGameOver_) {
		if (player_->GetIsDead()) {

			//ゲームオーバー演出を始める
			isGameOver_ = true;

			//スプライトを映す
			gameOverSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

			spaceKeySprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

			leftArrowSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

			rightArrowSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

			//シェイクを始める
			Shake::GetInstance()->Start(1.0f, 0.5f);

			//色を反転させる
			OffScreen::GetInstance()->SetColorReverseRatio(1.0f);
		}
	} else {

		//ゲームオーバー時スペースキーが押されたら
		if (Input::GetInstance()->IsTriggerPushKey(DIK_SPACE)) {

			//フェードアウトを始める
			Fade::GetInstance()->StartFadeOut();
		}
	}


	//フェードアウトが終わったら
	if (Fade::GetInstance()->GetState() == Fade::FADE_OUT_END) {

		//フェードの状態をリセット
		Fade::GetInstance()->SetState(Fade::NONE);

		//タイトルシーンに遷移
		SceneManager::GetInstance()->ChangeScene(SceneManager::kTitle);
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

	//ゲームオーバースプライトの描画
	gameOverSprite_->Draw(LayerType::UI);

	//スペースキースプライトの描画
	spaceKeySprite_->Draw(LayerType::UI);

	//左矢印スプライトの描画
	leftArrowSprite_->Draw(LayerType::UI);

	//右矢印スプライトの描画
	rightArrowSprite_->Draw(LayerType::UI);
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

///=====================================================/// 
/// スタート時のアニメーション
///=====================================================///
void GameScene::StartAnimation() {

	//スタート時以外はスキップ
	if (!isStart_) {
		return;
	}

	//タイマーを進ませる
	timer_ += 1.0f / 60.0f;

	if (animNum_ == 0) {

		//最初のキーは初期位置設定用
		player_->SetPosition(animPoints_[animNum_].playerPos);

		camera_->GetWorldTransform().rotate_ = animPoints_[animNum_].cameraRot;

		//キーを進ませる
		animNum_++;
	} else {

		//進捗を計算
		float t = (timer_ - animPoints_[animNum_ - 1].time) / (animPoints_[animNum_].time - animPoints_[animNum_ - 1].time);

		//1以上になったらそろえる
		if (t >= 1.0f) {

			t = 1.0f;
		}

		//キーフレーム間を補間
		player_->SetPosition(EaseOut(animPoints_[animNum_ - 1].playerPos, animPoints_[animNum_].playerPos, t, animPoints_[animNum_].mag));

		camera_->GetWorldTransform().rotate_ = EaseOut(animPoints_[animNum_ - 1].cameraRot, animPoints_[animNum_].cameraRot, t, animPoints_[animNum_].mag);

		//キーを進ませる
		if (t == 1.0f) {

			animNum_++;
		}

		//最後のキーが終わったら
		if (animNum_ == static_cast<int>(animPoints_.size())) {

			//スタート時の演出を終わる
			isStart_ = false;

			//追従カメラを有効化
			followCamera_->SetIsActive(true);

			//プレイヤーが動けるようにする
			player_->SetIsMoveActive(true);
		}
	}

}
#include "GameScene.h"

#include "Base/OffScreen.h"
#include "Base/Input.h"
#include "Scene/SceneManager.h"
#include "ObjectManager.h"

#include "2d/Sprite/SpriteManager.h"
#include "3d/Model/ModelManager.h"
#include "2d/Sprite/TextureManager.h"

#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"
#include "3d/Object/SkyBoxCommon.h"

#include "LevelEditor/LevelDataLoader.h"
#include "Fade/Fade.h"
#include "Shake/Shake.h"
#include "Flash/Flash.h"

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

	ObjectManager::GetInstance()->SpawnPlayer();

	player_ = ObjectManager::GetInstance()->GetPlayer();

	player_->Initialize(camera_.get(), bulletManager_.get(),false);

	//最初は動けないようにする
	player_->SetIsMoveActive(false);

	//エネミーマネージャーの初期化
	enemyManager_->Initialize(camera_.get(), bulletManager_.get(), player_);

	/// === 追尾カメラの生成 === ///

	followCamera_ = std::make_unique<FollowCamera>();

	followCamera_->Initialize(camera_.get(), player_);

	//最初は無効化する
	followCamera_->SetIsActive(false);

	/// === スプライトの読み込み === ///

	SpriteManager::GetInstance()->LoadSprite("GameOver", "RoadFlightGameOver");

	SpriteManager::GetInstance()->LoadSprite("GameOverSpace", "GameOverSpace");

	SpriteManager::GetInstance()->LoadSprite("GameOverArrow", "GameOverArrow");

	SpriteManager::GetInstance()->LoadSprite("GameClear", "RoadFlightGameClear");

	SpriteManager::GetInstance()->LoadSprite("GameClearSpace", "GameClearSpace");

	SpriteManager::GetInstance()->LoadSprite("GameClearArrow", "GameClearArrow");

	SpriteManager::GetInstance()->LoadSprite("KillToTitle", "KillToTitle");

	/// === ゲームオーバースプライトの生成 === ///

	//テキストスプライトの生成
	gameOverSprite_ = std::make_unique<Object2D>();

	gameOverSprite_->Initialize();

	gameOverSprite_->SetSprite("GameOver");

	gameOverSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameOverSprite_->SetTranslate({ 640.0f,100.0f });

	gameOverSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	//スペースキースプライトの生成
	gameOverSpaceSprite_ = std::make_unique<Object2D>();

	gameOverSpaceSprite_->Initialize();

	gameOverSpaceSprite_->SetSprite("GameOverSpace");

	gameOverSpaceSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameOverSpaceSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	gameOverSpaceSprite_->SetTranslate({ 640.0f,600.0f });

	gameOverSpaceSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	spaceKeyPos_ = gameOverSpaceSprite_->GetTranslate();

	spaceKeySize_ = gameOverSpaceSprite_->GetSize();

	//左矢印スプライトの生成
	gameOverLeftArrowSprite_ = std::make_unique<Object2D>();

	gameOverLeftArrowSprite_->Initialize();

	gameOverLeftArrowSprite_->SetSprite("GameOverArrow");

	gameOverLeftArrowSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameOverLeftArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	gameOverLeftArrowSprite_->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f,spaceKeyPos_.y });

	gameOverLeftArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	//右矢印スプライトの生成
	gameOverRightArrowSprite_ = std::make_unique<Object2D>();

	gameOverRightArrowSprite_->Initialize();

	gameOverRightArrowSprite_->SetSprite("GameOverArrow");

	gameOverRightArrowSprite_->GetSprite()->SetIsFlipX(true);

	gameOverRightArrowSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameOverRightArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	gameOverRightArrowSprite_->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f,spaceKeyPos_.y });

	gameOverRightArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	/// === ゲームクリアスプライトの生成 === ///

	//テキストスプライトの生成
	gameClearSprite_ = std::make_unique<Object2D>();

	gameClearSprite_->Initialize();

	gameClearSprite_->SetSprite("GameClear");

	gameClearSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameClearSprite_->SetTranslate({ 640.0f,100.0f });

	gameClearSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	//スペースキースプライトの生成
	gameClearSpaceSprite_ = std::make_unique<Object2D>();

	gameClearSpaceSprite_->Initialize();

	gameClearSpaceSprite_->SetSprite("GameClearSpace");

	gameClearSpaceSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameClearSpaceSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	gameClearSpaceSprite_->SetTranslate({ 640.0f,600.0f });

	gameClearSpaceSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	//左矢印スプライトの生成
	gameClearLeftArrowSprite_ = std::make_unique<Object2D>();

	gameClearLeftArrowSprite_->Initialize();

	gameClearLeftArrowSprite_->SetSprite("GameClearArrow");

	gameClearLeftArrowSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameClearLeftArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	gameClearLeftArrowSprite_->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f,spaceKeyPos_.y });

	gameClearLeftArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	//右矢印スプライトの生成
	gameClearRightArrowSprite_ = std::make_unique<Object2D>();

	gameClearRightArrowSprite_->Initialize();

	gameClearRightArrowSprite_->SetSprite("GameClearArrow");

	gameClearRightArrowSprite_->GetSprite()->SetIsFlipX(true);

	gameClearRightArrowSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameClearRightArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	gameClearRightArrowSprite_->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f,spaceKeyPos_.y });

	gameClearRightArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	helpSprite_ = std::make_unique<Object2D>();

	helpSprite_->Initialize();

	helpSprite_->SetSprite("KillToTitle");

	helpSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	helpSprite_->SetTranslate({ 640.0f,360.0f });

	/// === エミッターの生成 === ///

	//衝撃波エミッター(左)
	shockWaveLeftEmitter_ = std::make_unique<EmitterGroup>();

	shockWaveLeftEmitter_->Initialize(camera_.get());

	shockWaveLeftEmitter_->LoadEmitter("ShockWaveLeft");

	//衝撃波エミッター(右)

	shockWaveRightEmitter_ = std::make_unique<EmitterGroup>();

	shockWaveRightEmitter_->Initialize(camera_.get());

	shockWaveRightEmitter_->LoadEmitter("ShockWaveRight");

	/// === 他変数の設定 === ///

	timer_ = 0.0f;

	animNum_ = 0;

	isStart_ = true;

	isGameOver_ = false;

	isClear_ = false;

	isClearAnim_ = false;

	arrowLength_ = 20.0f;

	arrowTimer_ = 0.0f;

	timerDirection_ = 1.0f;

	//キーフレームの設定
	startAnimPoints_.push_back({ Vector3(0.0f,20.0f,-600.0f),Vector3(0.0f,-std::numbers::pi_v<float>,0.0f),0.0f,1.0f });
	startAnimPoints_.push_back({ Vector3(0.0f,20.0f,-600.0f),Vector3(0.0f,-std::numbers::pi_v<float>,0.0f),1.0f,1.0f });
	startAnimPoints_.push_back({ Vector3(0.0f,0.75f,0.0f),Vector3(0.2f,-std::numbers::pi_v<float>,0.0f),2.0f,3.0f });
	startAnimPoints_.push_back({ Vector3(0.0f,1.0f,0.0f),Vector3(0.2f,0.0f,0.0f),4.0f,4.0f });

	clearAnimPoints_.push_back({ Vector3(0.0f,0.0f,0.0f),Vector3(0.2f,0.0f,0.0f),0.0f,1.0f });
	clearAnimPoints_.push_back({ Vector3(0.0f,4.0f,0.0f),Vector3(0.2f,0.0f,0.0f),1.5f,2.0f });
	clearAnimPoints_.push_back({ Vector3(0.0f,40.0f,600.0f),Vector3(0.2f,0.0f,0.0f),2.5f,2.0f });

	Fade::GetInstance()->SetCamera(camera_.get());

	Fade::GetInstance()->SetPlayer(player_);

	Fade::GetInstance()->StartFadeIn();

}

///=====================================================/// 
/// 終了処理
///=====================================================///
void GameScene::Finalize() {

	ObjectManager::GetInstance()->ClearAll();

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

	if (Fade::GetInstance()->GetState() == Fade::FADE_IN_END) {
		Fade::GetInstance()->SetState(Fade::NONE);
	}

	//スタート時のアニメーションの更新
	StartAnimation();

	//クリア時のアニメーションの更新
	ClearAnimation();

	//追尾カメラの更新
	followCamera_->Update();

	//カメラをデバッグ状態で更新
	camera_->Update();

	if (!isClear_) {

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

	float lerpNum = EaseOut(0.0f, arrowLength_, arrowTimer_ / 1.0f, 2.0f);

	gameOverLeftArrowSprite_->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f - lerpNum,spaceKeyPos_.y });

	gameOverRightArrowSprite_->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f + lerpNum,spaceKeyPos_.y });

	gameClearLeftArrowSprite_->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f - lerpNum,spaceKeyPos_.y });

	gameClearRightArrowSprite_->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f + lerpNum,spaceKeyPos_.y });

	//ゲームオーバースプライトの更新
	gameOverSprite_->Update();
	gameOverSpaceSprite_->Update();
	gameOverLeftArrowSprite_->Update();
	gameOverRightArrowSprite_->Update();

	//ゲームクリアスプライトの更新
	gameClearSprite_->Update();
	gameClearSpaceSprite_->Update();
	gameClearLeftArrowSprite_->Update();
	gameClearRightArrowSprite_->Update();

	helpSprite_->Update();

	//右衝撃波エミッターの更新
	shockWaveRightEmitter_->Update();

	//左衝撃波エミッターの更新
	shockWaveLeftEmitter_->Update();

	//プレイヤーが倒されたとき
	if (!isGameOver_) {
		if (player_->GetIsDead()) {

			//ゲームオーバー演出を始める
			isGameOver_ = true;

			//スプライトを映す
			gameOverSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

			gameOverSpaceSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

			gameOverLeftArrowSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

			gameOverRightArrowSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

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

	if (ObjectManager::GetInstance()->GetKillCount() >= 30) {

		if (Fade::GetInstance()->GetState() != Fade::FADE_OUT && Fade::GetInstance()->GetState() != Fade::FADE_OUT_END) {

			if (!isClearAnim_) {

				if (!isClear_) {

					isClear_ = true;

					animNum_ = 1;

					timer_ = 0.0f;

					clearAnimPoints_[0].playerPos = player_->GetWorldPos();

					enemyManager_->StartClearUpdate();

					player_->SetIsMoveActive(false);

					//スプライトを映す
					gameClearSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

					gameClearSpaceSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

					gameClearLeftArrowSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

					gameClearRightArrowSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

					//シェイクを始める
					Shake::GetInstance()->Start(1.0f, 1.0f);

					//画面をフラッシュさせる
					Flash::GetInstance()->Start(0.5f, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

					//色を反転させる
					OffScreen::GetInstance()->SetColorReverseRatio(0.75f);
				}
			}
		}
	}

	if (isClear_) {

		if (Input::GetInstance()->IsTriggerPushKey(DIK_SPACE)) {

			isClearAnim_ = true;

			isClear_ = false;

			//スプライトを再度隠す
			gameClearSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

			gameClearSpaceSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

			gameClearLeftArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

			gameClearRightArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

			//画面をフラッシュさせる
			Flash::GetInstance()->Start(0.5f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

			//色を反転させる
			OffScreen::GetInstance()->SetColorReverseRatio(0.0f);
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

	if (!isClear_ && !isClearAnim_) {

		helpSprite_->Draw(LayerType::UI);
	}

	//右衝撃波エミッターの描画
	shockWaveRightEmitter_->Draw();

	//左衝撃波エミッターの描画
	shockWaveLeftEmitter_->Draw();

	//ゲームオーバースプライトの更新
	gameOverSprite_->Draw(LayerType::UI);
	gameOverSpaceSprite_->Draw(LayerType::UI);
	gameOverLeftArrowSprite_->Draw(LayerType::UI);
	gameOverRightArrowSprite_->Draw(LayerType::UI);

	//ゲームクリアスプライトの更新
	gameClearSprite_->Draw(LayerType::UI);
	gameClearSpaceSprite_->Draw(LayerType::UI);
	gameClearLeftArrowSprite_->Draw(LayerType::UI);
	gameClearRightArrowSprite_->Draw(LayerType::UI);

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
		player_->SetPosition(startAnimPoints_[animNum_].playerPos);

		camera_->GetWorldTransform().rotate_ = startAnimPoints_[animNum_].cameraRot;

		//キーを進ませる
		animNum_++;
	} else {

		//進捗を計算
		float t = (timer_ - startAnimPoints_[animNum_ - 1].time) / (startAnimPoints_[animNum_].time - startAnimPoints_[animNum_ - 1].time);

		//1以上になったらそろえる
		if (t >= 1.0f) {

			t = 1.0f;
		}

		//キーフレーム間を補間
		player_->SetPosition(EaseOut(startAnimPoints_[animNum_ - 1].playerPos, startAnimPoints_[animNum_].playerPos, t, startAnimPoints_[animNum_].mag));

		camera_->GetWorldTransform().rotate_ = EaseOut(startAnimPoints_[animNum_ - 1].cameraRot, startAnimPoints_[animNum_].cameraRot, t, startAnimPoints_[animNum_].mag);

		//キーを進ませる
		if (t == 1.0f) {

			animNum_++;
		}

		//最後のキーが終わったら
		if (animNum_ == static_cast<int>(startAnimPoints_.size())) {

			//スタート時の演出を終わる
			isStart_ = false;

			//追従カメラを有効化
			followCamera_->SetIsActive(true);

			//プレイヤーが動けるようにする
			player_->SetIsMoveActive(true);
		}
	}

}

void GameScene::ClearAnimation() {

	//クリア時以外はスキップ
	if (!isClearAnim_) {
		return;
	}

	//タイマーを進ませる
	timer_ += 1.0f / 60.0f;

	//進捗を計算
	float t = (timer_ - clearAnimPoints_[animNum_ - 1].time) / (clearAnimPoints_[animNum_].time - clearAnimPoints_[animNum_ - 1].time);

	//1以上になったらそろえる
	if (t >= 1.0f) {

		t = 1.0f;
	}

	//キーフレーム間を補間
	player_->SetPosition(EaseOut(clearAnimPoints_[animNum_ - 1].playerPos, clearAnimPoints_[animNum_].playerPos, t, clearAnimPoints_[animNum_].mag));

	shockWaveLeftEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(-1.75f, 0.0f, 0.0f);

	shockWaveRightEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(1.75f, 0.0f, 0.0f);

	//キーを進ませる
	if (t == 1.0f) {

		animNum_++;

		if (animNum_ == static_cast<int>(clearAnimPoints_.size()) - 1) {

			shockWaveLeftEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(-1.75f, 0.0f, 0.0f);
			shockWaveLeftEmitter_->Emit();

			shockWaveRightEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(1.75f, 0.0f, 0.0f);
			shockWaveRightEmitter_->Emit();

			//フェードアウトを始める
			Fade::GetInstance()->StartFadeOut();
		}
	}

	//最後のキーが終わったら
	if (animNum_ == static_cast<int>(clearAnimPoints_.size())) {

		isClearAnim_ = false;
	}

}
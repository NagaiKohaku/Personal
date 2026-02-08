#include "TitleScene.h"

#include <Scene/SceneManager.h>

#include <Base/OffScreen.h>
#include <Object/Manager/ObjectManager.h>
#include <Effect/Manager/EmitterManager.h>

#include <3d/Model/ModelManager.h>
#include <2d/Sprite/SpriteManager.h>
#include <Effect/Fade/Fade.h>
#include <Effect/Shake/Shake.h>

#include <Effect/Manager/UIManager.h>

#include <Base/Input.h>

#include <Math/Utility/Easing.h>

#include <imgui.h>

///=====================================================/// 
/// タイトルシーンの各種オブジェクトを初期化
///=====================================================///
void TitleScene::Initialize() {

	/// === カメラの設定 === ///

	//カメラを生成
	camera_ = std::make_unique<Camera>();

	//カメラの初期化
	camera_->Initialize();

	//デバッグカメラを使用しない
	camera_->SetDebugCameraFlag(false);

	//カメラの座標
	camera_->GetWorldTransform().translate_ = { 0.0f,1.0f,0.0f };

	camera_->SetOffsetZ(-20.0f);

	cameraRotate_ = { 0.3f,0.0f,0.0f };

	Shake::GetInstance()->SetCamera(camera_.get());

	EmitterManager::GetInstance()->SetCamera(camera_.get());

	/// === 3Dオブジェクトの設定 === ///

	//グラウンドマネージャーの生成
	groundManager_ = std::make_unique<GroundManager>();

	groundManager_->Initialize();

	//プレイヤーの生成
	ObjectManager::GetInstance()->SpawnPlayer();

	player_ = ObjectManager::GetInstance()->GetPlayer();

	player_->Initialize(camera_.get(),nullptr,false);

	player_->SetPosition({ 0.0f,1.0f,0.0f });

	/// === 2Dオブジェクトの設定 === ///

	UIManager::GetInstance()->LoadUI("TitleScene");

	spaceKeyPos_ = UIManager::GetInstance()->Get2DObject("Title", "SpaceButton")->GetTranslate();

	spaceKeySize_ = UIManager::GetInstance()->Get2DObject("Title", "SpaceButton")->GetSize();

	/// === エミッターの生成 === ///

	//衝撃波エミッター(左)
	shockWaveLeftEmitter_ = std::make_unique<EmitterGroup>();

	shockWaveLeftEmitter_->Initialize(camera_.get());

	shockWaveLeftEmitter_->LoadEmitter("ShockWaveLeft");

	//衝撃波エミッター(右)
	shockWaveRightEmitter_ = std::make_unique<EmitterGroup>();

	shockWaveRightEmitter_->Initialize(camera_.get());

	shockWaveRightEmitter_->LoadEmitter("ShockWaveRight");

	/// === その他 === ///

	//フェードにカメラとプレイヤーを設定
	Fade::GetInstance()->SetCamera(camera_.get());

	Fade::GetInstance()->SetPlayer(player_);

	//フェードイン開始
	Fade::GetInstance()->StartFadeIn();

	//アニメーション用の座標設定
	animPos_.emplace_back(Vector3(0.0f, 1.0f, 0.0f));

	animPos_.emplace_back(Vector3(0.0f, 3.0f, 0.0f));

	animPos_.emplace_back(Vector3(0.0, 30.0f, 400.0f));

	//アニメーション用の間隔設定
	animInterval.emplace_back(0.0f);

	animInterval.emplace_back(1.0f);

	animInterval.emplace_back(2.0f);

	//アニメーションタイマー初期化
	animTimer_ = 0.0f;

	//アニメーション番号初期化
	animNum_ = 0;

	//矢印の長さ
	arrowLength_ = 20.0f;

	//矢印のタイマー初期化
	arrowTimer_ = 0.0f;

	//タイマーの進行方向
	timerDirection_ = 1.0f;

	//スタートフラグ初期化
	isStart_ = false;

	//フェードフラグ初期化
	isFade_ = false;
}

///=====================================================/// 
/// タイトルシーン終了時に各種オブジェクトを解放
///=====================================================///
void TitleScene::Finalize() {

	//オブジェクトの破棄
	ObjectManager::GetInstance()->ClearAll();

	UIManager::GetInstance()->DeleteUI("Title");

	//フェードのカメラとプレイヤーを解除
	Fade::GetInstance()->SetCamera(nullptr);

	Fade::GetInstance()->SetPlayer(nullptr);

	//シェイクのカメラを解除
	Shake::GetInstance()->SetCamera(nullptr);
}

///=====================================================/// 
/// タイトルシーンの毎フレーム更新処理
///=====================================================///
void TitleScene::Update() {

	//スペースキーが押されたらスタート処理開始
	if (Input::GetInstance()->IsTriggerPushKey(DIK_SPACE)) {

		if (!isStart_) {

			if (!isFade_) {

				animTimer_ = 0.0f;

				isStart_ = true;
			}
		}
	}

	//色反転していたら徐々に戻す
	if (OffScreen::GetInstance()->GetColorReverseRatio() > 0.0f) {

		float currentNum = OffScreen::GetInstance()->GetColorReverseRatio();

		OffScreen::GetInstance()->SetColorReverseRatio(Lerp(currentNum, 0.0f, 0.1f));
	}

	if (!isFade_) {

		//スタート処理中なら
		if (isStart_) {

			Start();
		} else {

			//カメラを回転させる
			cameraRotate_.y += 0.01f;

			//2πを超えたら0に戻す
			if (cameraRotate_.y >= 3.14f * 2.0f) {

				cameraRotate_.y -= 3.14f * 2.0f;
			}

			//カメラの回転を設定
			camera_->SetRotate(cameraRotate_);
		}
	}

	//タイマーの進行
	arrowTimer_ += (1.0f / 60.0f) * timerDirection_;

	//タイマーが最大値・最小値を超えたら反転
	if (arrowTimer_ >= 1.0f) {

		arrowTimer_ = 1.0f;

		timerDirection_ *= -1.0f;
	}

	if (arrowTimer_ <= 0.0f) {

		arrowTimer_ = 0.0f;

		timerDirection_ *= -1.0f;
	}

	float lerpNum = EaseOut(0.0f, arrowLength_, arrowTimer_ / 1.0f, 2.0f);

	UIManager::GetInstance()->Get2DObject("Title", "LeftArrow")->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f - lerpNum,spaceKeyPos_.y });

	UIManager::GetInstance()->Get2DObject("Title", "RightArrow")->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f + lerpNum,spaceKeyPos_.y });

	//カメラの更新
	camera_->Update();

	//プレイヤーの更新
	player_->Update();

	//グラウンドマネージャーの更新
	groundManager_->Update();

	//衝撃波エミッターの更新
	shockWaveLeftEmitter_->Update();

	//衝撃波エミッターの更新
	shockWaveRightEmitter_->Update();

	if (isFade_) {

		//フェードアウトが終わったらシーンチェンジ
		if (Fade::GetInstance()->GetState() == Fade::FadeState::FADE_OUT_END) {

			Fade::GetInstance()->SetState(Fade::FadeState::NONE);

			SceneManager::GetInstance()->ChangeScene(SceneManager::SceneType::kGame);
		}
	}
}

///=====================================================/// 
/// タイトルシーンに必要な描画処理
///=====================================================///
void TitleScene::Draw() {

	//プレイヤーの描画
	player_->Draw();

	//グラウンドマネージャーの描画
	groundManager_->Draw();

	//衝撃波エミッターの描画(左)
	shockWaveLeftEmitter_->Draw();

	//衝撃波エミッターの描画(右)
	shockWaveRightEmitter_->Draw();

}

///=====================================================/// 
/// デバッグ
///=====================================================///
void TitleScene::ImGui() {

#ifdef _USE_IMGUI

#endif // _USE_IMGUI

}

///=====================================================/// 
/// タイトル開始時の演出アニメーションを管理
///=====================================================///
void TitleScene::Start() {

	//アニメーションタイマーの進行
	animTimer_ += 1.0f / 60.0f;

	//アニメーション間隔を超えたら次のアニメーションへ
	if (animTimer_ >= animInterval[animNum_]) {

		animTimer_ = 0.0f;

		animNum_ += 1;

		//最後のアニメーションなら衝撃波エミッターを発生させる
		if (animNum_ == static_cast<int>(animPos_.size()) - 1) {

			shockWaveLeftEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(-1.75f, 0.0f, 0.0f);
			shockWaveLeftEmitter_->Emit();

			shockWaveRightEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(1.75f, 0.0f, 0.0f);
			shockWaveRightEmitter_->Emit();

			if (Fade::GetInstance()->GetState() != Fade::FadeState::FADE_OUT) {
				if (Fade::GetInstance()->GetState() != Fade::FadeState::FADE_OUT_END) {

					Fade::GetInstance()->StartFadeOut();
				}
			}
		}

		//最後のアニメーションであればスタート演出を終了する
		if (animNum_ >= static_cast<int>(animPos_.size())) {

			animNum_ = static_cast<int>(animPos_.size()) - 1;

			isStart_ = false;

			isFade_ = true;

			return;
		}
	}

	//アニメーションの進行度合いを計算
	float t = animTimer_ / animInterval[animNum_];

	//プレイヤーの座標を取得
	Vector3 playerPos = player_->GetWorldPos();

	//プレイヤーの座標を補間して設定
	Vector3 pos = EaseOut(animPos_[animNum_ - 1], animPos_[animNum_], t, 2.0f);

	player_->SetPosition(pos);

	//衝撃波エミッターの座標をプレイヤーに追従させる
	shockWaveLeftEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(-1.75f, 0.0f, 0.0f);

	shockWaveRightEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(1.75f, 0.0f, 0.0f);

	//最初のアニメーションの時はスプライトを一緒にフェードアウトさせる
	if (animNum_ == 1) {

		UIManager::GetInstance()->Get2DObject("Title", "Title")->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,Lerp(1.0f,0.0f,t) });

		UIManager::GetInstance()->Get2DObject("Title", "SpaceButton")->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,Lerp(1.0f,0.0f,t) });

		UIManager::GetInstance()->Get2DObject("Title", "LeftArrow")->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,Lerp(1.0f,0.0f,t) });

		UIManager::GetInstance()->Get2DObject("Title", "RightArrow")->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,Lerp(1.0f,0.0f,t) });
	}

	//最後のアニメーションの時はカメラをプレイヤーに向ける
	if (animNum_ >= static_cast<int>(animPos_.size())) {

		Vector3 cameraPos = camera_->GetWorldTransform().GetWorldTranslate();

		Vector3 direction = Normalize(playerPos - cameraPos);

		Vector3 toPlayerRot = {
			-atan2f(direction.y, hypotf(direction.z, direction.x)),
			atan2f(direction.x,direction.z),
			0.0f
		};

		camera_->SetRotate(toPlayerRot);

	} else {

		Vector3 cameraRot = camera_->GetWorldTransform().rotate_;

		//カメラを前方に向ける
		camera_->SetRotate(EaseOut(cameraRot, Vector3(0.0f, 0.0f, 0.0f), t, 2.0f));
	}
}
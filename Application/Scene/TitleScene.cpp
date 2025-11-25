#include "TitleScene.h"

#include <Scene/SceneManager.h>

#include <Base/OffScreen.h>
#include <ObjectManager.h>

#include <3d/Model/ModelManager.h>
#include <2d/Sprite/SpriteManager.h>
#include <Fade/Fade.h>
#include <Shake/Shake.h>

#include <Base/Input.h>

#include <Math/Easing.h>

#include <imgui.h>

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

	/// === モデルの読み込み === ///

	SpriteManager::GetInstance()->LoadSprite("TitleSprite", "RoadflightTitle");

	SpriteManager::GetInstance()->LoadSprite("TitleSpace", "GameOverSpace");

	SpriteManager::GetInstance()->LoadSprite("TitleArrow", "GameOverArrow");

	/// === 3Dオブジェクトの設定 === ///

	ObjectManager::GetInstance()->SpawnPlayer();

	player_ = ObjectManager::GetInstance()->GetPlayer();

	player_->Initialize(camera_.get());

	player_->SetPosition({ 0.0f,1.0f,0.0f });

	groundManager_ = std::make_unique<GroundManager>();

	groundManager_->Initialize();

	/// === 2Dオブジェクトの設定 === ///

	titleSprite_ = std::make_unique<Object2D>();

	titleSprite_->Initialize();

	titleSprite_->SetSprite("TitleSprite");

	titleSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	titleSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	titleSprite_->SetTranslate({ 640.0f,100.0f });

	//スペースキースプライトの生成
	spaceKeySprite_ = std::make_unique<Object2D>();

	spaceKeySprite_->Initialize();

	spaceKeySprite_->SetSprite("TitleSpace");

	spaceKeySprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	spaceKeySprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	spaceKeySprite_->SetTranslate({ 640.0f,600.0f });

	spaceKeyPos_ = spaceKeySprite_->GetTranslate();

	spaceKeySize_ = spaceKeySprite_->GetSize();

	//左矢印スプライトの生成
	leftArrowSprite_ = std::make_unique<Object2D>();

	leftArrowSprite_->Initialize();

	leftArrowSprite_->SetSprite("TitleArrow");

	leftArrowSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	leftArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	leftArrowSprite_->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f,spaceKeyPos_.y });

	//右矢印スプライトの生成
	rightArrowSprite_ = std::make_unique<Object2D>();

	rightArrowSprite_->Initialize();

	rightArrowSprite_->SetSprite("TitleArrow");

	rightArrowSprite_->GetSprite()->SetIsFlipX(true);

	rightArrowSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	rightArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	rightArrowSprite_->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f,spaceKeyPos_.y });

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

	Fade::GetInstance()->SetCamera(camera_.get());

	Fade::GetInstance()->SetPlayer(player_);

	Fade::GetInstance()->StartFadeIn();

	animPos_.emplace_back(Vector3(0.0f, 1.0f, 0.0f));

	animPos_.emplace_back(Vector3(0.0f, 3.0f, 0.0f));

	animPos_.emplace_back(Vector3(0.0, 30.0f, 400.0f));

	animInterval.emplace_back(0.0f);

	animInterval.emplace_back(1.0f);

	animInterval.emplace_back(2.0f);

	animTimer_ = 0.0f;

	animNum_ = 0;

	arrowLength_ = 20.0f;

	arrowTimer_ = 0.0f;

	timerDirection_ = 1.0f;

	isStart_ = false;

	isFade_ = false;
}

void TitleScene::Finalize() {

	ObjectManager::GetInstance()->ClearAll();

	Fade::GetInstance()->SetCamera(nullptr);

	Fade::GetInstance()->SetPlayer(nullptr);

	Shake::GetInstance()->SetCamera(nullptr);

}

void TitleScene::Update() {

	if (Input::GetInstance()->IsTriggerPushKey(DIK_SPACE)) {

		if (!isStart_) {

			if (!isFade_) {

				animTimer_ = 0.0f;

				isStart_ = true;
			}
		}
	}

	if (OffScreen::GetInstance()->GetColorReverseRatio() > 0.0f) {

		float currentNum = OffScreen::GetInstance()->GetColorReverseRatio();

		OffScreen::GetInstance()->SetColorReverseRatio(Lerp(currentNum, 0.0f, 0.1f));
	}

	if (!isFade_) {

		if (isStart_) {

			Start();
		} else {

			cameraRotate_.y += 0.01f;

			if (cameraRotate_.y >= 3.14f * 2.0f) {

				cameraRotate_.y -= 3.14f * 2.0f;
			}

			camera_->SetRotate(cameraRotate_);

		}
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

	leftArrowSprite_->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f - lerpNum,spaceKeyPos_.y });

	rightArrowSprite_->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f + lerpNum,spaceKeyPos_.y });

	camera_->Update();

	player_->Update();

	groundManager_->Update();

	shockWaveLeftEmitter_->Update();

	shockWaveRightEmitter_->Update();

	titleSprite_->Update();

	spaceKeySprite_->Update();

	leftArrowSprite_->Update();

	rightArrowSprite_->Update();

	if (isFade_) {

		if (Fade::GetInstance()->GetState() == Fade::FadeState::FADE_OUT_END) {

			Fade::GetInstance()->SetState(Fade::FadeState::NONE);

			SceneManager::GetInstance()->ChangeScene(SceneManager::kGame);
		}
	}
}

void TitleScene::Draw() {

	player_->Draw();

	groundManager_->Draw();

	shockWaveLeftEmitter_->Draw();

	shockWaveRightEmitter_->Draw();

	titleSprite_->Draw(LayerType::UI);

	spaceKeySprite_->Draw(LayerType::UI);

	leftArrowSprite_->Draw(LayerType::UI);

	rightArrowSprite_->Draw(LayerType::UI);
}

void TitleScene::ImGui() {

#ifdef _USE_IMGUI

	ImGui::Begin("TitleScene");

	Vector4 color = titleSprite_->GetSprite()->GetColor();

	ImGui::SliderFloat("alpha", &color.w, 0.0f, 1.0f);

	titleSprite_->GetSprite()->SetColor(color);

	ImGui::End();

#endif // _USE_IMGUI

}

void TitleScene::Start() {

	animTimer_ += 1.0f / 60.0f;

	if (animTimer_ >= animInterval[animNum_]) {

		animTimer_ = 0.0f;

		animNum_ += 1;

		if (animNum_ == static_cast<int>(animPos_.size()) - 1) {

			shockWaveLeftEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(-1.75f, 0.0f, 0.0f);
			shockWaveLeftEmitter_->Emit();

			shockWaveRightEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(1.75f, 0.0f, 0.0f);
			shockWaveRightEmitter_->Emit();

			if (Fade::GetInstance()->GetState() != Fade::FADE_OUT) {
				if (Fade::GetInstance()->GetState() != Fade::FADE_OUT_END) {

					Fade::GetInstance()->StartFadeOut();
				}
			}
		}

		if (animNum_ >= static_cast<int>(animPos_.size())) {

			animNum_ = animPos_.size() - 1;

			isStart_ = false;

			isFade_ = true;

			return;
		}
	}

	float t = animTimer_ / animInterval[animNum_];

	Vector3 playerPos = player_->GetWorldPos();

	Vector3 pos = EaseOut(animPos_[animNum_ - 1], animPos_[animNum_], t, 2.0f);

	player_->SetPosition(pos);

	shockWaveLeftEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(-1.75f, 0.0f, 0.0f);

	shockWaveRightEmitter_->GetWorldTransform().translate_ = player_->GetWorldPos() + Vector3(1.75f, 0.0f, 0.0f);

	if (animNum_ == 1) {

		titleSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,Lerp(1.0f,0.0f,t) });

		spaceKeySprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,Lerp(1.0f,0.0f,t) });

		leftArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,Lerp(1.0f,0.0f,t) });

		rightArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,Lerp(1.0f,0.0f,t) });
	}

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

		camera_->SetRotate(EaseOut(cameraRot, Vector3(0.0f, 0.0f, 0.0f), t, 2.0f));
	}
}
#include "TitleScene.h"

#include <Scene/SceneManager.h>

#include <3d/Model/ModelManager.h>
#include <2d/Sprite/SpriteManager.h>

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

	/// === モデルの読み込み === ///

	SpriteManager::GetInstance()->LoadSprite("TitleSprite", "RoadflightTitle");

	/// === 3Dオブジェクトの設定 === ///

	player_ = std::make_unique<Player>();

	player_->Initialize(camera_.get());

	player_->SetPosition({ 0.0f,1.0f,0.0f });

	groundManager_ = std::make_unique<GroundManager>();

	groundManager_->Initialize();

	/// === 2Dオブジェクトの設定 === ///

	titleSprite_ = std::make_unique<Object2D>();

	titleSprite_->Initialize();

	titleSprite_->SetSprite("TitleSprite");

	titleSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	titleSprite_->SetTranslate({ 640.0f,150.0f });

	fade_ = std::make_unique<Fade>();

	fade_->Initialize();

	fade_->StartFadeIn();

	animPos_.emplace_back(player_->GetWorldPos());

	animPos_.emplace_back(Vector3(0.0f, 5.0f, 0.0f));

	animPos_.emplace_back(Vector3(0.0, 10.0f, 400.0f));

	animInterval.emplace_back(0.0f);

	animInterval.emplace_back(1.0f);

	animInterval.emplace_back(2.0f);

	animTimer_ = 0.0f;

	animNum_ = 0;

	isStart_ = false;

	isFade_ = false;
}

void TitleScene::Finalize() {

}

void TitleScene::Update() {

	if (Input::GetInstance()->IsTriggerPushKey(DIK_SPACE)) {

		if (!isStart_) {

			if (!isFade_) {

				isStart_ = true;
			}
		}
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

	camera_->Update();

	player_->Update();

	groundManager_->Update();

	titleSprite_->Update();

	fade_->Update();

	if (isFade_) {

		if (fade_->GetState() == Fade::FadeState::NONE) {

			SceneManager::GetInstance()->ChangeScene(SceneManager::kGame);
		}
	}
}

void TitleScene::Draw() {

	player_->Draw();

	groundManager_->Draw();

	titleSprite_->Draw(LayerType::UI);

	fade_->Draw();
}

void TitleScene::ImGui() {

	ImGui::Begin("TitleScene");

	Vector4 color = titleSprite_->GetSprite()->GetColor();

	ImGui::SliderFloat("alpha", &color.w, 0.0f, 1.0f);

	titleSprite_->GetSprite()->SetColor(color);

	ImGui::End();
}

void TitleScene::Start() {

	animTimer_ += 1.0f / 60.0f;

	if (animTimer_ >= animInterval[animNum_]) {

		animTimer_ = 0.0f;

		animNum_++;

		if (animNum_ >= animPos_.size()) {

			animNum_ = animPos_.size() - 1;

			isStart_ = false;

			isFade_ = true;

			fade_->StartFadeOut();

			return;
		}
	}

	float t = animTimer_ / animInterval[animNum_];

	Vector3 playerPos = player_->GetWorldPos();

	Vector3 pos = EaseOut(animPos_[animNum_ - 1], animPos_[animNum_], t, 2.0f);

	player_->SetPosition(pos);

	if (animNum_ >= animPos_.size()) {

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
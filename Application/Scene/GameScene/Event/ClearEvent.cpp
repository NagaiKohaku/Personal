#include "ClearEvent.h"

#include <UIManager.h>
#include <Flash/Flash.h>
#include <Shake/Shake.h>
#include <Fade/Fade.h>
#include <Base/OffScreen.h>
#include <Base/Input.h>

void ClearEvent::Start(Player* player, Camera* camera) {

	player_ = player;
	camera_ = camera;

	motionNum_ = 1;

	canMove_ = false;

	changeScene_ = true;

	motionPoint_.push_back({ player_->GetWorldPos(),camera_->GetWorldTransform().rotate_,0.0f,EaseType::LINEAR,1.0f });
	motionPoint_.push_back({ Vector3(0.0f,4.0f,0.0f),Vector3(0.0f,0.0f,0.0f),1.5f,EaseType::EASE_OUT,2.0f });
	motionPoint_.push_back({ Vector3(0.0f,40.0f,600.0f),Vector3(0.0f,0.0f,0.0f),2.5f,EaseType::EASE_OUT,2.0f });

	player_->SetIsMoveActive(false);

	//スプライトを映す
	UIManager::GetInstance()->GetUIGroup("Clear")->isActive = true;

	//テキスト系のUIを隠す
	UIManager::GetInstance()->GetUIGroup("Reticle")->isActive = false;

	UIManager::GetInstance()->GetUIGroup("Help")->isActive = false;

	//シェイクを始める
	Shake::GetInstance()->Start(1.0f, 1.0f);

	//画面をフラッシュさせる
	Flash::GetInstance()->Start(0.5f, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	//色を反転させる
	OffScreen::GetInstance()->SetColorReverseRatio(0.75f);
}

void ClearEvent::Exit() {
}

void ClearEvent::Update() {

	if (Input::GetInstance()->IsTriggerPushKey(DIK_SPACE)) {

		canMove_ = true;

		//クリアUIを隠す
		UIManager::GetInstance()->GetUIGroup("Clear")->isActive = false;

		//画面をフラッシュさせる
		Flash::GetInstance()->Start(0.5f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

		//色を反転させる
		OffScreen::GetInstance()->SetColorReverseRatio(0.0f);
	}

	if (canMove_) {

		UpdateEventMotion();
	}

	if (motionNum_ == static_cast<int>(motionPoint_.size()) - 1) {
		if (!isFade_) {

			Fade::GetInstance()->StartFadeOut();

			isFade_ = true;
		}
	}
}
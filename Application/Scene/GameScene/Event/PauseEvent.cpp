#include "PauseEvent.h"

#include <Effect/Manager/UIManager.h>
#include <Effect/Manager/EmitterManager.h>
#include <Effect/Fade/Fade.h>
#include <Base/Input.h>

using namespace MyEngine;

void PauseEvent::Start(EngineContext context, Player* player, FollowCamera* followCamera) {

	context_ = context;
	player_ = player;

	canMove_ = false;

	UIManager::GetInstance()->GetUIGroup("Reticle")->isActive = false;
	UIManager::GetInstance()->GetUIGroup("Help")->isActive = false;
	UIManager::GetInstance()->GetUIGroup("Pause")->isActive = true;

	EmitterManager::GetInstance()->SetIsUpdate(false);

	player_->SetIsMoveActive(false);

	timer_ = 0.0f;

	timerDirection_ = 1.0f;
}

void PauseEvent::Exit() {

	UIManager::GetInstance()->GetUIGroup("Reticle")->isActive = true;
	UIManager::GetInstance()->GetUIGroup("Help")->isActive = true;
	UIManager::GetInstance()->GetUIGroup("Pause")->isActive = false;

	EmitterManager::GetInstance()->SetIsUpdate(true);

	player_->SetIsMoveActive(true);
}

void PauseEvent::Update() {

	timer_ += (1.0f / 60.0f) * timerDirection_;

	if (timer_ >= 1.0f) {

		timer_ = 1.0f;

		timerDirection_ *= -1.0f;
	}

	if (timer_ <= 0.0f) {

		timer_ = 0.0f;

		timerDirection_ *= -1.0f;
	}

	float alphaNum = EaseOut(0.0f, 1.0f, timer_ / 1.0f);

	UIManager::GetInstance()->Get2DObject("Pause", "Text")->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, alphaNum));

	if (delay_) {

		if (context_.input->IsTriggerPushKey(DIK_ESCAPE)) {

			isFinished_ = true;
		}

		if (context_.input->IsTriggerPushKey(DIK_SPACE)) {

			Fade::GetInstance()->StartFadeOut();
		}
	}

	delay_ = true;
}

void PauseEvent::Draw() {
}

GameSceneEventBase::EventType PauseEvent::RequestNextEvent() const {

	if (isFinished_) {

		return EventType::GAME;
	}

	return EventType::NONE;
}

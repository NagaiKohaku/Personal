#include "PauseEvent.h"

#include <Effect/Manager/UIManager.h>
#include <Effect/Manager/EmitterManager.h>
#include <Effect/Fade/Fade.h>
#include <Base/Input.h>

using namespace MyEngine;

void PauseEvent::Start(Player* player, Camera* camera, FollowCamera* followCamera) {

	player_ = player;

	canMove_ = false;

	UIManager::GetInstance()->GetUIGroup("Reticle")->isActive = false;
	UIManager::GetInstance()->GetUIGroup("Help")->isActive = false;
	UIManager::GetInstance()->GetUIGroup("Pause")->isActive = true;

	EmitterManager::GetInstance()->SetIsUpdate(false);

	player_->SetIsMoveActive(false);
}

void PauseEvent::Exit() {

	UIManager::GetInstance()->GetUIGroup("Reticle")->isActive = true;
	UIManager::GetInstance()->GetUIGroup("Help")->isActive = true;
	UIManager::GetInstance()->GetUIGroup("Pause")->isActive = false;

	EmitterManager::GetInstance()->SetIsUpdate(true);

	player_->SetIsMoveActive(true);
}

void PauseEvent::Update() {

	if (delay_) {

		if (Input::GetInstance()->IsTriggerPushKey(DIK_ESCAPE)) {

			isFinished_ = true;
		}

		if (Input::GetInstance()->IsTriggerPushKey(DIK_SPACE)) {

			Fade::GetInstance()->StartFadeOut();
		}
	}

	delay_ = true;
}

GameSceneEventBase::EventType PauseEvent::RequestNextEvent() const {

	if (isFinished_) {

		return EventType::GAME;
	}

	return EventType::NONE;
}

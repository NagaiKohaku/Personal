#include "GameEvent.h"

#include <Base/Input.h>
#include <Object/Manager/ObjectManager.h>

using namespace MyEngine;

void GameEvent::Start(Player* player, Camera* camera, FollowCamera* followCamera) {

	player_ = player;
	camera_ = camera;
	followCamera_ = followCamera;

	canMove_ = true;

	followCamera_->SetIsActive(true);

	player_->SetIsMoveActive(true);
}

void GameEvent::Exit() {
}

void GameEvent::Update() {
}

GameSceneEventBase::EventType GameEvent::RequestNextEvent() const {

	if (Input::GetInstance()->IsTriggerPushKey(DIK_ESCAPE)) {

		return EventType::PAUSE;
	}

	if (ObjectManager::GetInstance()->GetKillCount() >= 30) {

		return EventType::CLEAR;
	}

	if (player_->GetIsDead()) {

		return EventType::GAMEOVER;
	}

	return EventType::NONE;
}

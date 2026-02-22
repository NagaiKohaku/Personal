#include "GameEvent.h"

#include <Base/Input.h>
#include <Object/Manager/ObjectManager.h>

using namespace MyEngine;

void GameEvent::Start(MyEngine::EngineContext context, Player* player, FollowCamera* followCamera) {

	context_ = context;
	player_ = player;
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

	if (context_.input->IsTriggerPushKey(DIK_ESCAPE)) {

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

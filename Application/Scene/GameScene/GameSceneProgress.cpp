#include "GameSceneProgress.h"

#include <Scene/GameScene/Event/GameSceneEventFactory.h>

using namespace MyEngine;

void GameSceneProgress::Initialize(MyEngine::EngineContext context, Player* player, FollowCamera* followCamera) {

	context_ = context;

	player_ = player;

	followCamera_ = followCamera;

	event_ = GameSceneEventFactory::Create(GameSceneEventBase::EventType::START);

	event_->Start(context_, player_, followCamera_);

}

void GameSceneProgress::Update() {

	event_->Update();

	GameSceneEventBase::EventType nextType = event_->RequestNextEvent();

	if (nextType != GameSceneEventBase::EventType::NONE) {
		ChangeEvent(nextType);
	}
}

void GameSceneProgress::Draw() {

	event_->Draw();
}

void GameSceneProgress::ChangeEvent(GameSceneEventBase::EventType type) {

	event_->Exit();

	event_ = GameSceneEventFactory::Create(type);

	event_->Start(context_, player_, followCamera_);
}
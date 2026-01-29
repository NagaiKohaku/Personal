#include "GameSceneProgress.h"

#include <Scene/GameScene/Event/GameSceneEventFactory.h>

void GameSceneProgress::Initialize(Player* player, Camera* camera, FollowCamera* followCamera) {

	player_ = player;

	camera_ = camera;

	followCamera_ = followCamera;

	event_ = GameSceneEventFactory::Create(GameSceneEventBase::EventType::START);

	event_->Start(player_, camera_, followCamera_);

}

void GameSceneProgress::Update() {

	event_->Update();

	GameSceneEventBase::EventType nextType = event_->RequestNextEvent();

	if (nextType != GameSceneEventBase::EventType::NONE) {
		ChangeEvent(nextType);
	}
}

void GameSceneProgress::ChangeEvent(GameSceneEventBase::EventType type) {

	event_->Exit();

	event_ = GameSceneEventFactory::Create(type);

	event_->Start(player_, camera_, followCamera_);
}
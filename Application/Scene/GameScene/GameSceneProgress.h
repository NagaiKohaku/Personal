#pragma once

#include <Object/Camera/FollowCamera.h>
#include <Object/Player/Player.h>

#include <Scene/EngineContext.h>
#include <Scene/GameScene/Event/GameSceneEventBase.h>

#include <memory>

class GameSceneProgress {

public:

	void Initialize(MyEngine::EngineContext context, Player* player, FollowCamera* followCamera);

	void Update();

private:

	void ChangeEvent(GameSceneEventBase::EventType type);

private:

	std::unique_ptr<GameSceneEventBase> event_;

	MyEngine::EngineContext context_;

	Player* player_;

	FollowCamera* followCamera_;

public:

	bool canMove() { return event_->canMove(); }
};
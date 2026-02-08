#pragma once

#include <3d/Camera/Camera.h>
#include <Object/Camera/FollowCamera.h>
#include <Object/Player/Player.h>

#include <Scene/GameScene/Event/GameSceneEventBase.h>

#include <memory>

class GameSceneProgress {

public:

	void Initialize(Player* player, Camera* camera, FollowCamera* followCamera);

	void Update();

private:

	void ChangeEvent(GameSceneEventBase::EventType type);

private:

	std::unique_ptr<GameSceneEventBase> event_;

	Player* player_;

	Camera* camera_;

	FollowCamera* followCamera_;

public:

	bool canMove() { return event_->canMove(); }
};
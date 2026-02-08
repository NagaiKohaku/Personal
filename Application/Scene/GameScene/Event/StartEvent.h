#pragma once

#include <Scene/GameScene/Event/GameSceneEventBase.h>

#include <Object/Camera/FollowCamera.h>

class StartEvent : public GameSceneEventBase {

public:

	void Start(Player* player, Camera* camera, FollowCamera* followCamera) override;

	void Exit() override;

	void Update() override;

	EventType RequestNextEvent() const override;

};
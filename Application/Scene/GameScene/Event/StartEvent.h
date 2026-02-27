#pragma once

#include <Scene/GameScene/Event/GameSceneEventBase.h>

#include <Object/Camera/FollowCamera.h>

class StartEvent : public GameSceneEventBase {

public:

	void Start(EngineContext context, Player* player, FollowCamera* followCamera) override;

	void Exit() override;

	void Update() override;

	void Draw() override;

	EventType RequestNextEvent() const override;

};
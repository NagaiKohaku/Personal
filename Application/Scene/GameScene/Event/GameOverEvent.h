#pragma once

#include <Scene/GameScene/Event/GameSceneEventBase.h>

class GameOverEvent : public GameSceneEventBase {

public:

	void Start(Player* player, MyEngine::Camera* camera, FollowCamera* followCamera) override;

	void Exit() override;

	void Update() override;

	EventType RequestNextEvent() const override;

private:

	bool isFade_ = false;
};
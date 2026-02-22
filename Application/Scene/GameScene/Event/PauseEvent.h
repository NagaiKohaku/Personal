#pragma once

#include <Scene/GameScene/Event/GameSceneEventBase.h>

class PauseEvent : public GameSceneEventBase {

public:

	void Start(MyEngine::EngineContext context, Player* player, FollowCamera* followCamera) override;

	void Exit() override;

	void Update() override;

	EventType RequestNextEvent() const override;

private:

	bool delay_ = false;

};
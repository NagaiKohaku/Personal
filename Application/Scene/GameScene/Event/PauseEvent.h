#pragma once

#include <Scene/GameScene/Event/GameSceneEventBase.h>

class PauseEvent : public GameSceneEventBase {

public:

	void Start(MyEngine::EngineContext context, Player* player, FollowCamera* followCamera) override;

	void Exit() override;

	void Update() override;

	void Draw() override;

	EventType RequestNextEvent() const override;

private:

	float timer_;

	//アニメーションタイマーの進行方向
	float timerDirection_;

	bool delay_ = false;

};
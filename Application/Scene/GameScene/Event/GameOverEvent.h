#pragma once

#include <Scene/GameScene/Event/GameSceneEventBase.h>

class GameOverEvent : public GameSceneEventBase {

public:

	void Start(EngineContext context, Player* player, FollowCamera* followCamera) override;

	void Exit() override;

	void Update() override;

	void Draw() override;

	EventType RequestNextEvent() const override;

private:

	//スペースキースプライトの位置
	MyEngine::Vector2 spaceKeyPos_;

	//スペースキースプライトの大きさ
	MyEngine::Vector2 spaceKeySize_;

	//矢印の長さ
	float arrowLength_;

	//矢印のタイマー
	float arrowTimer_;

	//アニメーションタイマーの進行方向
	float timerDirection_;

	bool isFade_ = false;
};
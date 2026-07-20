#pragma once

#include <Scene/GameScene/Event/GameSceneEventBase.h>

class ClearEvent : public GameSceneEventBase {

public:

	void Start(MyEngine::EngineContext context, Player* player, FollowCamera* followCamera) override;

	void Exit() override;

	void Update() override;

	void Draw() override;

	EventType RequestNextEvent() const override;

private:

	//衝撃波エミッター(左)
	std::unique_ptr<MyEngine::EmitterGroup> shockWaveLeftEmitter_;

	//衝撃波エミッター(右)
	std::unique_ptr<MyEngine::EmitterGroup> shockWaveRightEmitter_;

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

	float sceneChangeTimer_ = 0.0f;

	float sceneChangeMaxTime_ = 1.0f;

};
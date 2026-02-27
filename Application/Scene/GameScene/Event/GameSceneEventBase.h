#pragma once

#include <Scene/EngineContext.h>
#include <Object/Player/Player.h>
#include <Object/Camera/FollowCamera.h>

#include <Math/Vector/Vector3.h>
#include <Math/Utility/Easing.h>

#include <vector>

class GameSceneEventBase {

public:

	enum class EventType {
		NONE,
		START,
		GAME,
		PAUSE,
		CLEAR,
		GAMEOVER
	};

protected:

	struct EventMotionPoint {
		MyEngine::Vector3 playerPosition;
		MyEngine::Vector3 cameraRotation;
		float duration;
		MyEngine::EaseType easeType;
		float easeMag;
	};

public:
	virtual ~GameSceneEventBase() = default;

	virtual void Start(EngineContext context, Player* player, FollowCamera* followCamera) = 0;   // 開始時1回
	virtual void Exit() = 0;    // 終了処理
	virtual void Update() = 0;  // 毎フレーム
	virtual void Draw() = 0;

	virtual EventType RequestNextEvent() const { return EventType::NONE; }

protected:

	void UpdateEventMotion();

protected:

	EngineContext context_;

	Player* player_ = nullptr;

	FollowCamera* followCamera_ = nullptr;

	std::vector<EventMotionPoint> motionPoint_;

	float motionTimer_ = 0.0f;

	int motionNum_ = 0;

	bool isFinished_ = false;

	bool canMove_ = true;

public:

	bool IsFinished() const { return isFinished_; }

	bool canMove() const { return canMove_; }

};
#pragma once

#include <Player/Player.h>
#include <3d/Camera/Camera.h>
#include <Camera/FollowCamera.h>

#include <Math/Vector3.h>
#include <Math/Easing.h>

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
		Vector3 playerPosition;
		Vector3 cameraRotation;
		float duration;
		EaseType easeType;
		float easeMag;
	};

public:
	virtual ~GameSceneEventBase() = default;

	virtual void Start(Player* player, Camera* camera, FollowCamera* followCamera) = 0;   // 開始時1回
	virtual void Exit() = 0;    // 終了処理
	virtual void Update() = 0;  // 毎フレーム

	virtual EventType RequestNextEvent() const { return EventType::NONE; }

protected:

	void UpdateEventMotion();

protected:

	Player* player_ = nullptr;

	Camera* camera_ = nullptr;

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
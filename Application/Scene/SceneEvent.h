#pragma once

#include <Player/Player.h>
#include <3d/Camera/Camera.h>

#include <Math/Vector3.h>
#include <Math/Easing.h>

#include <vector>

class SceneEvent {

protected:

	struct EventMotionPoint {
		Vector3 playerPosition;
		Vector3 cameraRotation;
		float duration;
		EaseType easeType;
		float easeMag;
	};

public:
	virtual ~SceneEvent() = default;

	virtual void Start(Player* player, Camera* camera) = 0;   // 開始時1回
	virtual void Exit() = 0;    // 終了処理
	virtual void Update() = 0;  // 毎フレーム

protected:

	void UpdateEventMotion();

protected:

	Player* player_ = nullptr;

	Camera* camera_ = nullptr;

	std::vector<EventMotionPoint> motionPoint_;

	float motionTimer_ = 0.0f;

	int motionNum_ = 0;

	bool isFinished_ = false;

	bool canMove_ = true;

	bool changeScene_ = false;

public:

	bool IsFinished() const { return isFinished_; }

	bool canMove() const { return canMove_; }

	bool changeScene() const { return changeScene_; }
};
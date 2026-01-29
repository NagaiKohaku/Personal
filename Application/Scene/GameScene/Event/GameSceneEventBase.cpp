#include "GameSceneEventBase.h"

#include <algorithm>

void GameSceneEventBase::UpdateEventMotion() {

	if (motionPoint_.empty()) {
		return;
	}

	if (isFinished_) {
		return;
	}

	//タイマーを進ませる
	motionTimer_ += 1.0f / 60.0f;

	EventMotionPoint from = motionPoint_[motionNum_ - 1];
	EventMotionPoint to = motionPoint_[motionNum_];

	//進捗を計算
	float t = (motionTimer_ - from.duration) / (to.duration - from.duration);

	t = std::clamp(t, 0.0f, 1.0f);

	//キーフレーム間を補間
	player_->SetPosition(Ease(from.playerPosition, to.playerPosition, t, to.easeType, to.easeMag));

	camera_->GetWorldTransform().rotate_ = Ease(from.cameraRotation, to.cameraRotation, t, to.easeType, to.easeMag);

	//キーを進ませる
	if (t == 1.0f) {

		motionNum_++;
	}

	//最後のキーが終わったら
	if (motionNum_ == static_cast<int>(motionPoint_.size())) {

		isFinished_ = true;
	}
}
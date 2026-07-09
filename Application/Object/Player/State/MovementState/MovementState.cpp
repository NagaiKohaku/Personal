#include "MovementState.h"

#include "Math/Vector/Vector3.h"
#include "Object/Player/Player.h"
#include "Math/Utility/Easing.h"

#include "imgui.h"

#include <algorithm>

using namespace MyEngine;

void MovementState::Update(Player* player) {

	player_ = player;

	Vector3 velocity = player->GetInputDirection() * moveSpeed_;

	movePos_ = player->GetCoreWorldTransform().translate_ + velocity;

	movePos_ = {
		std::clamp(movePos_.x,initialPos_.x - moveRange_.x,initialPos_.x + moveRange_.x),
		std::clamp(movePos_.y,initialPos_.y - moveRange_.y,initialPos_.y + moveRange_.y),
		std::clamp(movePos_.z,initialPos_.z - moveRange_.z,initialPos_.z + moveRange_.z),
	};

	moveRotate_ = {
		-Normalize(velocity).y,
		0.0f,
		-Normalize(velocity).x
	};

	moveRotate_ = {
		std::clamp(moveRotate_.x,-rotRange_.x,rotRange_.x),
		std::clamp(moveRotate_.y,-rotRange_.y,rotRange_.y),
		std::clamp(moveRotate_.z,-rotRange_.z,rotRange_.z)
	};

	CoreMoveUpdate();

	LeftWingMoveUpdate();

	RightWingMoveUpdate();

}

void MovementState::CoreMoveUpdate() {

	WorldTransform coreWT = player_->GetCoreWorldTransform();

	//コアオブジェクトの移動・回転
	coreWT.translate_ = Lerp(coreWT.translate_, movePos_, moveStrength_ / 100.0f);
	coreWT.rotate_ = Lerp(coreWT.rotate_, moveRotate_, rotStrength_ / 100.0f);

	//地面より下に行かないようにする
	coreWT.translate_.y = fmaxf(1.0f, coreWT.translate_.y);

	coreWT.translate_ = coreWT.translate_;

	//移動後のワールド座標を設定
	player_->SetCoreWorldTransform(coreWT);
}

void MovementState::LeftWingMoveUpdate() {

	WorldTransform leftWingWT = player_->GetLeftWingWorldTransform();

	WorldTransform leftTrailWT = player_->GetLeftTrailWorldTransform();

	leftWingWT.translate_ = EaseOut(leftWingWT.translate_, leftWingTranslate_, 0.1f, 2.0f);
	leftWingWT.rotate_ = EaseOut(leftWingWT.rotate_, leftWingRotate_, 0.1f, 2.0f);

	leftTrailWT.translate_ = EaseOut(leftTrailWT.translate_, leftWingTrailPos_, 0.1f, 2.0f);

	player_->SetLeftWingWorldTransform(leftWingWT);

	player_->SetLeftTrailWorldTransform(leftTrailWT);
}

void MovementState::RightWingMoveUpdate() {

	WorldTransform rightWingWT = player_->GetRightWingWorldTransform();

	WorldTransform rightTrailWT = player_->GetRightTrailWorldTransform();

	rightWingWT.translate_ = EaseOut(rightWingWT.translate_, rightWingTranslate_, 0.1f, 2.0f);
	rightWingWT.rotate_ = EaseOut(rightWingWT.rotate_, rightWingRotate_, 0.1f, 2.0f);

	rightTrailWT.translate_ = EaseOut(rightTrailWT.translate_, rightWingTrailPos_, 0.1f, 2.0f);

	player_->SetRightWingWorldTransform(rightWingWT);

	player_->SetRightTrailWorldTransform(rightTrailWT);
}

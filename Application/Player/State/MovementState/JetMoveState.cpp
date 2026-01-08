#include "JetMoveState.h"

#include <Player/Player.h>

#include <Math/Easing.h>

#include <algorithm>

void JetMoveState::Enter() {

	initialPos_ = { 0.0f,2.0f,0.0f };

	moveRange_ = { 7.0f,4.0f,0.0f };

	rotRange_ = { 0.5f,0.0f,0.3f };

	moveStrength_ = 10.0f;

	rotStrength_ = 10.0f;
}

void JetMoveState::Update(Player& player, Vector3 velocity) {

	if (player.GetWorldPos().y <= 1.0f) {

		player.ChangeTankState();

		return;
	}

	WorldTransform coreWT = player.GetCoreWorldTransform();
	WorldTransform leftWingWT = player.GetLeftWingWorldTransform();
	WorldTransform rightWingWT = player.GetRightWingWorldTransform();

	//コアオブジェクトの現在角度
	Vector3 playerPos = coreWT.translate_;
	Vector3 playerRot = coreWT.rotate_;

	//左ウィングの現在座標・回転
	Vector3 leftWingPos = leftWingWT.translate_;
	Vector3 leftWingRot = leftWingWT.rotate_;

	//右ウィングの現在座標・回転
	Vector3 rightWingPos = rightWingWT.translate_;
	Vector3 rightWingRot = rightWingWT.rotate_;

	////右トレイルの現在座標
	//Vector3 rightTrailPos = rightTrail_->GetWorldTransform().translate_;

	////左トレイルの現在座標
	//Vector3 leftTrailPos = leftTrail_->GetWorldTransform().translate_;

	//移動後座標の算出
	Vector3 movePos = playerPos + velocity;

	movePos = {
		std::clamp(movePos.x,initialPos_.x - moveRange_.x,initialPos_.x + moveRange_.x),
		std::clamp(movePos.y,initialPos_.y - moveRange_.y,initialPos_.y + moveRange_.y),
		std::clamp(movePos.z,initialPos_.z - moveRange_.z,initialPos_.z + moveRange_.z),
	};

	//左右移動でZ軸回転、上下移動でX軸回転をするように設定
	Vector3 rotate = {
		-Normalize(velocity).y,
		0.0f,
		-Normalize(velocity).x
	};

	//回転範囲で制限
	rotate = {
		std::clamp(rotate.x,-rotRange_.x,rotRange_.x),
		std::clamp(rotate.y,-rotRange_.y,rotRange_.y),
		std::clamp(rotate.z,-rotRange_.z,rotRange_.z)
	};

	//コアオブジェクトの回転
	coreWT.translate_ = Lerp(playerPos, movePos, moveStrength_ / 100.0f);
	coreWT.rotate_ = Lerp(playerRot, rotate, rotStrength_ / 100.0f);

	coreWT.translate_.y = fmaxf(1.0f, coreWT.translate_.y);

	player.SetCoreWorldTransform(coreWT);

	//右ウィングの移動・回転
	rightWingWT.translate_ = EaseOut(rightWingPos, Vector3(1.35f, 0.0f, 0.0f), 0.1f, 2.0f);
	rightWingWT.rotate_ = EaseOut(rightWingRot, Vector3(0.0f, 0.0f, 0.0f), 0.1f, 2.0f);

	player.SetRightWingWorldTransform(rightWingWT);

	//左ウィングの移動・回転
	leftWingWT.translate_ = EaseOut(leftWingPos, Vector3(-1.35f, 0.0f, 0.0f), 0.1f, 2.0f);
	leftWingWT.rotate_ = EaseOut(leftWingRot, Vector3(0.0f, 0.0f, 0.0f), 0.1f, 2.0f);

	player.SetLeftWingWorldTransform(leftWingWT);

	////右トレイルの移動
	//rightTrail_->GetWorldTransform().translate_ = EaseOut(rightTrailPos, Vector3(0.75f, 0.0f, 0.0f), 0.1f, 2.0f);

	////左トレイルの移動
	//leftTrail_->GetWorldTransform().translate_ = EaseOut(leftTrailPos, Vector3(-0.75f, 0.0f, 0.0f), 0.1f, 2.0f);

}
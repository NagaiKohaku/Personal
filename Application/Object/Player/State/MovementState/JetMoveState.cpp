#include "JetMoveState.h"

#include <Object/Player/Player.h>

#include <Math/Utility/Easing.h>

#include <algorithm>

using namespace MyEngine;

void JetMoveState::Enter(Input* inputPtr) {

	MovementState::Enter(inputPtr);

	initialPos_ = { 0.0f,2.0f,0.0f };

	moveRange_ = { 7.0f,4.0f,0.0f };

	rotRange_ = { 0.5f,0.0f,0.3f };

	moveSpeed_ = 1.0f;

	moveStrength_ = 10.0f;

	rotStrength_ = 10.0f;

	minJetHeight_ = 1.0f;
}

void JetMoveState::Update(Player& player) {

	//高度が一定値を下回ったら戦車モードに変更
	if (player.GetWorldPos().y <= minJetHeight_) {

		player.ChangeTankState();

		return;
	}

	//各オブジェクトのワールド変換取得
	WorldTransform coreWT = player.GetCoreWorldTransform();
	WorldTransform leftWingWT = player.GetLeftWingWorldTransform();
	WorldTransform rightWingWT = player.GetRightWingWorldTransform();
	WorldTransform leftTrailWT = player.GetLeftTrailWorldTransform();
	WorldTransform rightTrailWT = player.GetRightTrailWorldTransform();

	//コアオブジェクトの現在角度
	Vector3 playerPos = coreWT.translate_;
	Vector3 playerRot = coreWT.rotate_;

	//左ウィングの現在座標・回転
	Vector3 leftWingPos = leftWingWT.translate_;
	Vector3 leftWingRot = leftWingWT.rotate_;

	//右ウィングの現在座標・回転
	Vector3 rightWingPos = rightWingWT.translate_;
	Vector3 rightWingRot = rightWingWT.rotate_;

	//左トレイルの現在座標
	Vector3 leftTrailPos = leftTrailWT.translate_;

	//右トレイルの現在座標
	Vector3 rightTrailPos = rightTrailWT.translate_;

	//移動量の計算
	Vector3 velocity = player.GetInputDirection() * moveSpeed_;

	//移動後座標の計算
	Vector3 movePos = playerPos + velocity;

	//移動範囲で制限
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

	/// === コアオブジェクトの移動・回転 === ///

	//コアオブジェクトの移動・回転
	coreWT.translate_ = Lerp(playerPos, movePos, moveStrength_ / 100.0f);
	coreWT.rotate_ = Lerp(playerRot, rotate, rotStrength_ / 100.0f);

	//地面より下に行かないようにする
	coreWT.translate_.y = fmaxf(minJetHeight_, coreWT.translate_.y);

	//移動後のワールド座標を設定
	player.SetCoreWorldTransform(coreWT);

	/// === 左ウィングの移動・回転 === ///

	//左ウィングの移動・回転
	leftWingWT.translate_ = EaseOut(leftWingPos, Vector3(0.0f, 0.0f, 0.0f), 0.1f, 2.0f);
	leftWingWT.rotate_ = EaseOut(leftWingRot, Vector3(0.0f, 0.0f, 0.0f), 0.1f, 2.0f);

	//移動後のワールド座標を設定
	player.SetLeftWingWorldTransform(leftWingWT);

	/// === 右ウィングの移動・回転 === ///

	//右ウィングの移動・回転
	rightWingWT.translate_ = EaseOut(rightWingPos, Vector3(0.0f, 0.0f, 0.0f), 0.1f, 2.0f);
	rightWingWT.rotate_ = EaseOut(rightWingRot, Vector3(0.0f, 0.0f, 0.0f), 0.1f, 2.0f);

	//移動後のワールド座標を設定
	player.SetRightWingWorldTransform(rightWingWT);

	/// === 左トレイルの移動 === ///

	//左トレイルの移動
	leftTrailWT.translate_ = EaseOut(leftTrailPos, Vector3(-0.75f, 0.0f, 0.0f), 0.1f, 2.0f);

	//移動後のワールド座標を設定
	player.SetLeftTrailWorldTransform(leftTrailWT);

	/// === 右トレイルの移動 === ///

	//右トレイルの移動
	rightTrailWT.translate_ = EaseOut(rightTrailPos, Vector3(0.75f, 0.0f, 0.0f), 0.1f, 2.0f);

	//移動後のワールド座標を設定
	player.SetRightTrailWorldTransform(rightTrailWT);
}
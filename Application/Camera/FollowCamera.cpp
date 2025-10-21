#include "FollowCamera.h"

#include "Math/Easing.h"

///=====================================================/// 
/// 初期化
///=====================================================///
void FollowCamera::Initialize(Camera* camera, Player* player) {

	//カメラポインタを取得
	camera_ = camera;

	//プレイヤーポインタを取得
	player_ = player;

	//オフセットを設定
	offset_ = { 0.0f,3.0f,0.0f };

	//追尾範囲を設定
	followRange_ = 10.0f;

	//追尾強度を設定
	followStrength_ = 10.0f;

	//カメラの座標を設定
	camera_->GetWorldTransform().translate_ = offset_;

	camera_->SetOffsetZ(-20.0f);
}

///=====================================================/// 
/// 更新
///=====================================================///
void FollowCamera::Update() {

	if (camera_->IsDebugCamera()) {
		return;
	}

	if (!isActive_) {
		return;
	}

	//カメラの現在座標
	Vector3 cameraPos = camera_->GetWorldTransform().GetWorldTranslate();

	//プレイヤーの現在座標
	Vector3 playerPos = player_->GetWorldPos();

	//プレイヤーの移動量
	Vector3 playerVelocity = Normalize(player_->GetVelocity());

	//カメラからプレイヤーへの方向
	Vector3 direction = Normalize(playerPos - cameraPos);

	//プレイヤーのいる方向
	Vector3 toPlayerRot = {
		-atan2f(direction.y, hypotf(direction.z, direction.x)),
		atan2f(direction.x,direction.z),
		0.0f
	};

	//プレイヤーの傾き
	Vector3 playerSlope = {
		playerVelocity.y * -0.25f,
		0.0f,
		playerVelocity.x * -0.25f
	};

	//プレイヤーが戦車状態であれば傾きはなし
	if (player_->GetMoveState() == Player::MOVESTATE::TANK) {

		playerSlope = { 0.0f,0.0f,0.0f };
	}

	//カメラの座標を設定
	camera_->GetWorldTransform().translate_ = offset_;

	//カメラの角度を線形補間で計算
	camera_->GetWorldTransform().rotate_ = Lerp(
		camera_->GetWorldTransform().rotate_,
		(toPlayerRot + playerSlope) * (followRange_ / 100.0f),
		followStrength_ / 100.0f
	);

}
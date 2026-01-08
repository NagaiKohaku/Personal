#include "FollowCamera.h"

#include "Math/Easing.h"

///=====================================================/// 
/// カメラをプレイヤーに追従させるために初期化
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

	//傾き強度を設定
	slopeStrength_ = 25.0f;

	//プレイヤーとカメラの距離を設定
	distanceToPlayer_ = 20.0f;

	//カメラの座標を設定
	camera_->GetWorldTransform().translate_ = offset_;

	//カメラの距離を設定
	camera_->SetOffsetZ(-distanceToPlayer_);
}

///=====================================================/// 
/// プレイヤーにカメラを追従させるために毎フレーム更新
///=====================================================///
void FollowCamera::Update() {

	//デバッグカメラであれば追従処理を行わない
	if (camera_->IsDebugCamera()) {
		return;
	}

	//無効化されていたら処理を行わない
	if (!isActive_) {
		return;
	}

	/// === プレイヤーの方向を計算 === ///

	//カメラの現在座標
	Vector3 cameraPos = camera_->GetWorldTransform().GetWorldTranslate();

	//プレイヤーの現在座標
	Vector3 playerPos = player_->GetWorldPos();

	//カメラからプレイヤーへの方向
	Vector3 direction = Normalize(playerPos - cameraPos);

	//プレイヤーのいる方向
	Vector3 toPlayerRot = {
		-atan2f(direction.y, hypotf(direction.z, direction.x)),
		atan2f(direction.x,direction.z),
		0.0f
	};

	/// === プレイヤーの移動方向からカメラの傾きを計算 === ///

	//プレイヤーの移動量
	Vector3 playerVelocity = Normalize(player_->GetInputDirection());

	//カメラの傾き
	Vector3 cameraSlope = { 0.0f,0.0f,0.0f };

	//プレイヤーが戦闘機状態の時のみ傾きを計算
	if (player_->GetMoveState() == Player::MOVESTATE::JET) {

		cameraSlope = {
			playerVelocity.y * -(slopeStrength_ / 100.0f),
			0.0f,
			playerVelocity.x * -(slopeStrength_ / 100.0f)
		};
	}

	//カメラの座標を設定
	camera_->GetWorldTransform().translate_ = offset_;

	//カメラの角度を線形補間で計算
	camera_->GetWorldTransform().rotate_ = Lerp(
		camera_->GetWorldTransform().rotate_,
		(toPlayerRot + cameraSlope) * (followRange_ / 100.0f),
		followStrength_ / 100.0f
	);

}
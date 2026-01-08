#include "EnemyDeadState.h"

#include <Enemy/Enemy.h>

#include <Math/Easing.h>

#include <numbers>

void EnemyDeadState::Enter() {

	timer_ = 0.0f;

	maxTime_ = 5.0f;

	moveStrength_ = 10.0f;

	deadMoveVelocity_ = Vector3(0.0f, -5.0f, -10.0f);

	isDestroy_ = false;
}

void EnemyDeadState::Update(Enemy& enemy) {

	//タイマーを進ませる
	timer_ += 1.0f / 60.0f;

	//タイマーが終了時間に達したら
	if (timer_ >= maxTime_) {

		//削除処理を開始
		enemy.StartRemove();

		return;
	}

	//地面に到達したら
	if (enemy.GetWorldTransform().translate_.y <= 0.0f) {

		//まだ破壊処理を開始していなければ
		if (!isDestroy_) {

			//破壊処理を開始
			enemy.StartDestroy();

			//破壊処理を開始したフラグを立てる
			isDestroy_ = true;
		}
	}

	//タイマーの比率
	float ratio = timer_ / maxTime_;

	//敵のワールド座標取得
	WorldTransform enemyWT = enemy.GetWorldTransform();

	//目標地点を計算
	Vector3 targetPos = Lerp(enemyWT.translate_, enemyWT.translate_ + deadMoveVelocity_, ratio);

	//目標角度を計算
	Vector3 targetRot = Lerp(
		Vector3(0.0f, -std::numbers::pi_v<float>, 0.0f),
		Vector3(0.0f, -std::numbers::pi_v<float> * 10.0f, std::numbers::pi_v<float> *10.0f),
		ratio
	);

	//目標地点まで移動
	enemyWT.translate_ = Lerp(enemy.GetWorldTransform().translate_, targetPos, moveStrength_ / 100.0f);

	//目標角度まで回転
	enemyWT.rotate_ = Lerp(enemy.GetWorldTransform().rotate_, targetRot, moveStrength_ / 100.0f);

	//移動後のワールド座標を設定
	enemy.SetWorldTransform(enemyWT);
}

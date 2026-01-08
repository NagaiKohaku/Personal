#include "EnemyEntryState.h"

#include <Enemy/State/EnemyMoveState.h>

#include <Enemy/Enemy.h>

#include <Math/Easing.h>

#include <numbers>

void EnemyEntryState::Enter() {

	timer_ = 0.0f;

	maxTime_ = 1.0f;

	moveStrength_ = 10.0f;
}

void EnemyEntryState::Update(Enemy& enemy) {

	//タイマーを進ませる
	timer_ += 1.0f / 60.0f;

	//タイマーが終了時間に達したら
	if (timer_ >= maxTime_) {

		//前進状態に変更
		enemy.ChangeState(std::make_unique<EnemyMoveState>());

		return;
	}

	//タイマーの比率
	float ratio = timer_ / maxTime_;

	//敵のワールド座標取得
	WorldTransform enemyWT = enemy.GetWorldTransform();

	//目標地点を計算
	Vector3 targetPos = EaseOutBack(enemy.GetEntryPos(), enemy.GetStandbyPos(), ratio, 2.0f);

	//前フレームからの移動量を計算
	//前方を向くようにZ方向成分を強めに設定
	Vector3 velocity = enemy.GetPrePos() - targetPos + Vector3(0.0f, 0.0f, 10.0f);

	//目標角度を計算
	Vector3 targetRot = {
		Normalize(velocity).y,
		-std::numbers::pi_v<float>,
		Normalize(velocity).x
	};

	//目標地点へ移動
	enemyWT.translate_ = Lerp(enemy.GetWorldTransform().translate_,targetPos, moveStrength_ / 100.0f);

	//目標角度へ回転
	enemyWT.rotate_ = Lerp(enemy.GetWorldTransform().rotate_, targetRot, moveStrength_ / 100.0f);

	//移動後のワールド座標を設定
	enemy.SetWorldTransform(enemyWT);
}

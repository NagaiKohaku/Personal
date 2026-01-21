#include "EnemyAttackState.h"

#include <Enemy/State/EnemyExitState.h>

#include <Enemy/Enemy.h>
#include <Player/Player.h>
#include <Bullet/BulletManager.h>
#include <ObjectManager.h>

#include <Math/Easing.h>

#include <numbers>

void EnemyAttackState::Enter() {

	maxTime_ = 2.0f;

	attackFrequency_ = 1.0f;

	timer_ = 0.0f;

	attackTimer_ = attackFrequency_;
}

void EnemyAttackState::Update(Enemy& enemy) {

	//タイマーを進ませる
	timer_ += 1.0f / 60.0f;

	//攻撃タイマーを進ませる
	attackTimer_ += 1.0f / 60.0f;

	//タイマーが終了時間に達したら
	if (timer_ >= maxTime_) {

		//撤退状態に変更
		enemy.ChangeState(std::make_unique<EnemyExitState>());

		return;
	}

	//タイマーの比率
	float ratio = timer_ / maxTime_;

	//プレイヤーのポインタを取得
	Player* player = ObjectManager::GetInstance()->GetPlayer();

	//攻撃可能になったら
	if (attackTimer_ >= attackFrequency_) {

		//プレイヤーへの方向
		Vector3 direction = player->GetWorldPos() - enemy.GetWorldPos();

		//プレイヤーに向かって弾を発射
		enemy.GetBulletManager()->AddBullet(
			enemy.GetWorldPos(),
			Normalize(direction),
			BulletManager::BulletType::ENEMY
		);

		//攻撃タイマーのリセット
		attackTimer_ = 0.0f;
	}
}
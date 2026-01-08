#include "TankAttackState.h"

#include <Player/Player.h>
#include <Player/LockOn.h>
#include <Bullet/BulletManager.h>

#include <Enemy/Enemy.h>
#include <ObjectManager.h>
#include <Base/Input.h>
#include <Math/MakeMatrixMath.h>

void TankAttackState::Enter() {

	//攻撃間隔を設定
	attackInterval_ = 1.0f;

	//タイマーをリセット
	attackTimer_ = attackInterval_;

	//ロックオン範囲を設定
	lockOnRange_ = 300.0f;
}

void TankAttackState::Update(Player& player, LockOn& lockOn, BulletManager& bulletManager) {

	std::list<Enemy*> enemyList = ObjectManager::GetInstance()->GetEnemies();

	attackTimer_ += 1.0f / 60.0f;

	//タイマーが攻撃間隔を超えたら
	if (attackTimer_ >= attackInterval_) {

		for (auto& enemy : enemyList) {

			//敵が死亡していたらスキップ
			if (enemy->CheckIsDead()) {

				continue;
			}

			//ロックオン範囲内であれば
			if (lockOn.GetMainReticleToEnemyLength(enemy) <= lockOnRange_) {

				//ロックオン対象に追加
				lockOn.AddLockOnEnemy(enemy);
			}
		}

		//スペースキーが押されていたら
		if (Input::GetInstance()->isPushKey(DIK_SPACE)) {

			//マズルフラッシュエミッターを発生
			player.EmitMuzzleFlash();

			//レティクルの位置を取得
			std::vector<Vector3> reticlePositions = lockOn.GetLockOnReticlePos();

			for (auto& pos : reticlePositions) {

				//オブジェクトからレティクルへの方向
				Vector3 direction = pos - player.GetWorldPos();

				//バレットマネージャーに弾を追加
				bulletManager.AddBullet(
					player.GetWorldPos(),
					Normalize(direction),
					BulletManager::BULLETTYPE::TANK
				);
			}

			//攻撃のタイマーをリセット
			attackTimer_ = 0.0f;
		}
	}
}
#include "JetAttackState.h"

#include <Player/Player.h>
#include <Player/LockOn.h>
#include <Bullet/BulletManager.h>

#include <Base/Input.h>

void JetAttackState::Enter() {

	//攻撃間隔を設定
	attackInterval_ = 0.1f;

	//タイマーをリセット
	attackTimer_ = attackInterval_;
}

void JetAttackState::Update(Player& player, LockOn& lockOn, BulletManager& bulletManager) {

	attackTimer_ += 1.0f / 60.0f;

	//タイマーが攻撃間隔を超えたら
	if (attackTimer_ >= attackInterval_) {

		//スペースキーが押されていたら
		if (Input::GetInstance()->isPushKey(DIK_SPACE)) {

			//マズルフラッシュエミッターを発生
			player.EmitMuzzleFlash();

			//オブジェクトからレティクルへの方向
			Vector3 direction = lockOn.GetMainReticlePos() - player.GetWorldPos();

			//バレットマネージャーに弾を追加
			bulletManager.AddBullet(
				player.GetWorldPos(),
				Normalize(direction),
				BulletManager::BULLETTYPE::JET
			);

			//攻撃のタイマーをリセット
			attackTimer_ = 0.0f;
		}
	}
}
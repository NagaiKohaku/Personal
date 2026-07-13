#include "TankAttackState.h"

#include <Object/Player/Player.h>
#include <Base/Input.h>
#include <Math/Utility/MakeMatrixMath.h>

using namespace MyEngine;

void TankAttackState::Enter() {

	//攻撃間隔を設定
	attackInterval_ = 1.0f;

	//タイマーをリセット
	attackTimer_ = attackInterval_;

	//ロックオン範囲を設定
	lockOnRange_ = 300.0f;
}

void TankAttackState::Update(Player& player) {

	attackTimer_ += 1.0f / 60.0f;

	//ロックオンの更新
	player.UpdateLockOn(lockOnRange_);

	//タイマーが攻撃間隔を超えたら
	if (attackTimer_ >= attackInterval_) {

		//スペースキーが押されていたら
		if (Input::GetInstance()->isPushKey(DIK_SPACE)) {

			//攻撃実行
			player.TankAttack();

			//攻撃のタイマーをリセット
			attackTimer_ = 0.0f;
		}
	}
}
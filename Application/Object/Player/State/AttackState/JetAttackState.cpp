#include "JetAttackState.h"

#include <Object/Player/Player.h>

#include <Base/Input.h>

void JetAttackState::Enter() {

	//攻撃間隔を設定
	attackInterval_ = 0.1f;

	//タイマーをリセット
	attackTimer_ = attackInterval_;
}

void JetAttackState::Update(Player& player) {

	attackTimer_ += 1.0f / 60.0f;

	//タイマーが攻撃間隔を超えたら
	if (attackTimer_ >= attackInterval_) {

		//スペースキーが押されていたら
		if (Input::GetInstance()->isPushKey(DIK_SPACE)) {

			//攻撃実行
			player.JetAttack();

			//攻撃のタイマーをリセット
			attackTimer_ = 0.0f;
		}
	}
}
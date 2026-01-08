#pragma once

#include <Enemy/State/EnemyState.h>

class EnemyAttackState : public EnemyState {

public:

	// 状態に入るときの処理
	void Enter() override;

	// 毎フレームの更新処理
	void Update(Enemy& enemy) override;

private:

	float timer_;

	float maxTime_;

	float attackTimer_;

	float attackFrequency_;
};


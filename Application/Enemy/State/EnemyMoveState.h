#pragma once

#include <Enemy/State/EnemyState.h>

class EnemyMoveState : public EnemyState {

public:

	// 状態に入るときの処理
	void Enter() override;

	// 毎フレームの更新処理
	void Update(Enemy& enemy) override;

private:

	float timer_;

	float maxTime_;

	float blinkStartTime_;

	float moveStrength_;

	bool startBlink_;
};


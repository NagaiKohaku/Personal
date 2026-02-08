#pragma once

#include <Object/Enemy/State/EnemyState.h>

class EnemyEntryState : public EnemyState {

public:

	// 状態に入るときの処理
	void Enter() override;

	// 毎フレームの更新処理
	void Update(Enemy& enemy) override;

private:

	float timer_;

	float maxTime_;

	float moveStrength_;
};
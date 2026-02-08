#pragma once

#include <Object/Enemy/State/EnemyState.h>
#include <Math/Vector/Vector3.h>

class EnemyDeadState : public EnemyState {

public:

	// 状態に入るときの処理
	void Enter() override;

	// 毎フレームの更新処理
	void Update(Enemy& enemy) override;

private:

	float timer_;

	float maxTime_;

	float moveStrength_;

	MyEngine::Vector3 deadMoveVelocity_;

	bool isDestroy_;
};


#pragma once

#include <Player/State/AttackState/AttackState.h>

class TankAttackState : public AttackState {

public:

	void Enter() override;

	void Update(Player& player, LockOn& lockOn, BulletManager& bulletManager) override;

private:

	float attackTimer_;

	float attackInterval_;

	float lockOnRange_;
};
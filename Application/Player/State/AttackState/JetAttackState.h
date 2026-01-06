#pragma once

#include <Player/State/AttackState/AttackState.h>

class JetAttackState : public AttackState {

public:

	void Enter() override;

	void Update(Player& player, LockOn& lockOn, BulletManager& bulletManager) override;

private:

	float attackTimer_;

	float attackInterval_;

};
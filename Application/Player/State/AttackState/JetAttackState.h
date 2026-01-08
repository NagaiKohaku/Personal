#pragma once

#include <Player/State/AttackState/AttackState.h>

class JetAttackState : public AttackState {

public:

	void Enter() override;

	void Update(Player& player) override;

private:

	float attackTimer_;

	float attackInterval_;

};
#pragma once

#include <Object/Player/State/AttackState/AttackState.h>

class TankAttackState : public AttackState {

public:

	void Enter() override;

	void Update(Player& player) override;

private:

	float lockOnRange_;
};
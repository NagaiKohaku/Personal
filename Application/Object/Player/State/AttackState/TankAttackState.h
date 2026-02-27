#pragma once

#include <Object/Player/State/AttackState/AttackState.h>

class TankAttackState : public AttackState {

public:

	void Enter(MyEngine::Input* inputPtr) override;

	void Update(Player& player) override;

private:

	float lockOnRange_;
};
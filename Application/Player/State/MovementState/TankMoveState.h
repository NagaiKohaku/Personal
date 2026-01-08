#pragma once

#include <Player/State/MovementState/MovementState.h>

#include <Math/Vector3.h>

class TankMoveState : public MovementState {

public:

	void Enter() override;

	void Update(Player& player) override;

private:

	Vector3 initialPos_;

	Vector3 moveRange_;

	Vector3 rotRange_;

	float moveSpeed_;

	float moveStrength_;

	float rotStrength_;

	float maxTankHeight_;
};
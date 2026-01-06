#pragma once

#include <Player/State/MovementState/MovementState.h>

class TankMoveState : public MovementState {

public:

	void Enter() override;

	void Update(Player& player, Vector3 velocity) override;

private:

	Vector3 initialPos_;

	Vector3 moveRange_;

	Vector3 rotRange_;

	float moveStrength_;

	float rotStrength_;
};
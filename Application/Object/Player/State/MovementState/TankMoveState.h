#pragma once

#include <Object/Player/State/MovementState/MovementState.h>

#include <Math/Vector/Vector3.h>

class TankMoveState : public MovementState {

public:

	void Enter() override;

	void Update(Player* player) override;

private:

	MyEngine::Vector3 initialPos_;

	MyEngine::Vector3 moveRange_;

	MyEngine::Vector3 rotRange_;

	float moveSpeed_;

	float moveStrength_;

	float rotStrength_;

	float maxTankHeight_;
};
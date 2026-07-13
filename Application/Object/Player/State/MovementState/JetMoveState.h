#pragma once

#include <Object/Player/State/MovementState/MovementState.h>

#include <Math/Vector/Vector3.h>

class JetMoveState : public MovementState {

public:

	void Enter() override;

	void Update(Player* player) override;
};
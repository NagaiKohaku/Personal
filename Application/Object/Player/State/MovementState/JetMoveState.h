#pragma once

#include <Object/Player/State/MovementState/MovementState.h>

#include <Math/Vector/Vector3.h>

class JetMoveState : public MovementState {

public:

	void Enter(MyEngine::Input* inputPtr) override;

	void Update(Player& player) override;

private:

	MyEngine::Vector3 initialPos_;

	MyEngine::Vector3 moveRange_;

	MyEngine::Vector3 rotRange_;

	float moveSpeed_;

	float moveStrength_;

	float rotStrength_;

	float minJetHeight_;
};
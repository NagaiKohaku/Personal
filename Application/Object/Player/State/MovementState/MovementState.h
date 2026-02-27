#pragma once

#include <Base/Input.h>

class Player;

class MovementState {

public:

	virtual ~MovementState() = default;

	virtual void Enter(MyEngine::Input* inputPtr) = 0;

	virtual void Update(Player& player) = 0;

protected:

	MyEngine::Input* input_;
};
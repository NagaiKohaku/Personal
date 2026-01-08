#pragma once

#include <Math/Vector3.h>

class Player;

class MovementState {

public:

	virtual ~MovementState() = default;

	virtual void Enter() = 0;

	virtual void Update(Player& player, Vector3 velocity) = 0;
};
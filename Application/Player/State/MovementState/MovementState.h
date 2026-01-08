#pragma once

class Player;

class MovementState {

public:

	virtual ~MovementState() = default;

	virtual void Enter() = 0;

	virtual void Update(Player& player) = 0;
};
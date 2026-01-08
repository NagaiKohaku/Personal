#pragma once

class Player;
class LockOn;
class BulletManager;

class AttackState {

public:

	virtual ~AttackState() = default;

	virtual void Enter() = 0;

	virtual void Update(Player& player) = 0;
};
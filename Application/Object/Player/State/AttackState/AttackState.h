#pragma once

#include <Base/Input.h>

class Player;
class LockOn;
class BulletManager;

class AttackState {

public:

	virtual ~AttackState() = default;

	virtual void Enter(MyEngine::Input* inputPtr);

	virtual void Update(Player& player) = 0;

protected:

	MyEngine::Input* input_;

	float attackTimer_;

	float attackInterval_;

public:

	float GetAttackTimer() const { return attackTimer_; }

	float GetAttackInterval() const { return attackInterval_; }

};
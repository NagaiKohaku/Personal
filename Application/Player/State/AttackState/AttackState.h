#pragma once

class Player;
class LockOn;
class BulletManager;

class AttackState {

public:

	virtual ~AttackState() = default;

	virtual void Enter() = 0;

	virtual void Update(Player& player) = 0;

protected:

	float attackTimer_;

	float attackInterval_;

public:

	float GetAttackTimer() const { return attackTimer_; }

	float GetAttackInterval() const { return attackInterval_; }

};
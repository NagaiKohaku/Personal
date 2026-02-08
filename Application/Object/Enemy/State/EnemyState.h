#pragma once

class Enemy;

class EnemyState {

public:

	virtual ~EnemyState() = default;

	virtual void Enter() = 0;

	virtual void Update(Enemy& enemy) = 0;
};
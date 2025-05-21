#pragma once

#include "3d/Object/Object3D.h"

#include "memory"

class BulletManager;

class Player {

public:

	void Initialize();

	void Update();

	void Draw();

public:

	void SetBulletManager(BulletManager* bulletManager) {
		bulletManager_ = bulletManager;
	}

private:

	void Move();

	void Attack();

private:

	BulletManager* bulletManager_ = nullptr;

	std::unique_ptr<Object3D> player_ = nullptr;

	float moveSpeed_ = 0.1f;

	float attackInterval_ = 0.5f;

	float attackTimer_ = 0.0f;
};
#pragma once

#include "3d/Object/Object3D.h"

#include "memory"

class BulletBase {

public:

	virtual ~BulletBase() = default;

	virtual void Initialize(Vector3 pos) = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

public:

	bool IsDead() const { return isDead_; }

protected:

	std::unique_ptr<Object3D> bullet_ = nullptr;

	//弾の移動速度
	float speed_ = 0.0f;

	//弾の移動量
	Vector3 velocity_ = { 0.0f,0.0f,0.0f };

	//弾の寿命
	float lifeTime_ = 0.0f;

	//弾の最大寿命
	float lifeTimeMax_ = 0.0f;

	//消滅フラグ
	bool isDead_ = false;
};
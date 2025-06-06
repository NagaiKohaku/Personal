#pragma once

#include "Bullet/BulletBase.h"

#include "Collider/SphereCollider.h"

#include "memory"

class PlayerBullet : public BulletBase {

public:
	void Initialize(Vector3 pos) override;

	void Update() override;

	void Draw() override;

private:

	void Move();

private:

	std::unique_ptr<SphereCollider> collider_;

};
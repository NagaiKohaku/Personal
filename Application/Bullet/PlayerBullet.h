#pragma once

#include "Bullet/BulletBase.h"

class PlayerBullet : public BulletBase {

public:
	void Initialize(Vector3 pos) override;

	void Update() override;

	void Draw();

private:

	void Move();
};
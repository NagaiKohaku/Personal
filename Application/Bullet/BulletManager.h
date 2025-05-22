#pragma once

#include "Bullet/BulletBase.h"

#include "list"

class BulletManager {

public:

	void Initialize();

	void Update();

	void Draw();

	void AddBullet(Vector3 pos);

private:

	std::list<std::unique_ptr<BulletBase>> bullets_;
};
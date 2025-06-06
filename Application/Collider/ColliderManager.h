#pragma once

#include "Collider.h"

#include "list"
#include "memory"

class ColliderManager {

public:

	static ColliderManager* GetInstance();

	void Initialize();

	void Update();

	void Draw();

	void AddCollider(Collider* collider);

private:

	void CheckAllCollision();

	bool CheckPair(Collider* first, Collider* second);

	bool IsCollisionAABBWithAABB(Collider* first, Collider* second);

	bool IsCollisionSphereWithSphere(Collider* first, Collider* second);

	bool IsCollisionAABBWithSphere(Collider* first, Collider* second);

private:

	std::list<Collider*> colliders_;
};
#include "ColliderManager.h"
#include "AABBCollider.h"
#include "SphereCollider.h"

#include "algorithm"

ColliderManager* ColliderManager::GetInstance() {
	static ColliderManager instance;
	return &instance;
}

void ColliderManager::Initialize() {

	colliders_.clear();
}

void ColliderManager::Update() {

	CheckAllCollision();
}

void ColliderManager::Draw() {
}

void ColliderManager::AddCollider(Collider* collider) {

	colliders_.push_back(collider);
}

void ColliderManager::CheckAllCollision() {

	for (auto it1 = colliders_.begin(); it1 != colliders_.end(); ++it1) {
		if (!(*it1)->GetIsActive()) {
			continue;
		}
		for (auto it2 = std::next(it1); it2 != colliders_.end(); ++it2) {
			if (!(*it2)->GetIsActive()) {
				continue;
			}
			if (CheckPair(*it1, *it2)) {

			}
		}
	}
}

bool ColliderManager::CheckPair(Collider* first, Collider* second) {

	if (dynamic_cast<AABBCollider*>(first) && dynamic_cast<AABBCollider*>(second)) {
		return IsCollisionAABBWithAABB(first, second);
	}
	if (dynamic_cast<SphereCollider*>(first) && dynamic_cast<SphereCollider*>(second)) {
		return IsCollisionSphereWithSphere(first, second);
	}
	if ((dynamic_cast<AABBCollider*>(first) && dynamic_cast<SphereCollider*>(second))) {
		return IsCollisionAABBWithSphere(first, second);
	}
	if ((dynamic_cast<SphereCollider*>(first) && dynamic_cast<AABBCollider*>(second))) {
		return IsCollisionAABBWithSphere(second, first);
	}

	return false;
}

bool ColliderManager::IsCollisionAABBWithAABB(Collider* first, Collider* second) {

	AABBCollider* firstAABBCol = dynamic_cast<AABBCollider*>(first);
	AABBCollider* secondAABBCol = dynamic_cast<AABBCollider*>(second);

	AABB firstAABB = {
		firstAABBCol->GetAABB().min + first->GetWorldTransform().GetWorldTranslate(),
		firstAABBCol->GetAABB().max + first->GetWorldTransform().GetWorldTranslate()
	};

	AABB secondAABB = {
		secondAABBCol->GetAABB().min + second->GetWorldTransform().GetWorldTranslate(),
		secondAABBCol->GetAABB().max + second->GetWorldTransform().GetWorldTranslate()
	};

	if (firstAABB.min.x <= secondAABB.max.x && firstAABB.max.x >= secondAABB.min.x &&
		firstAABB.min.y <= secondAABB.max.y && firstAABB.max.y >= secondAABB.min.y &&
		firstAABB.min.z <= secondAABB.max.z && firstAABB.max.z >= secondAABB.min.z) {

		return true;
	}

	return false;
}

bool ColliderManager::IsCollisionSphereWithSphere(Collider* first, Collider* second) {

	SphereCollider* firstSphereCol = dynamic_cast<SphereCollider*>(first);
	SphereCollider* secondSphereCol = dynamic_cast<SphereCollider*>(second);

	Sphere firstSphere = {
		firstSphereCol->GetSphere().center + firstSphereCol->GetWorldTransform().GetWorldTranslate(),
		firstSphereCol->GetSphere().radius
	};

	Sphere secondSphere = {
		secondSphereCol->GetSphere().center + secondSphereCol->GetWorldTransform().GetWorldTranslate(),
		secondSphereCol->GetSphere().radius
	};

	float distance = Length(firstSphere.center - secondSphere.center);

	if (distance <= firstSphere.radius + secondSphere.radius) {
		return true;
	}

	return false;
}

bool ColliderManager::IsCollisionAABBWithSphere(Collider* aabb, Collider* sphere) {

	AABBCollider* aabbCol = dynamic_cast<AABBCollider*>(aabb);
	SphereCollider* sphereCol = dynamic_cast<SphereCollider*>(sphere);

	AABB firstAABB = {
		aabbCol->GetAABB().min + aabb->GetWorldTransform().GetWorldTranslate(),
		aabbCol->GetAABB().max + aabb->GetWorldTransform().GetWorldTranslate()
	};

	Sphere secondSphere = {
		sphereCol->GetSphere().center + sphereCol->GetWorldTransform().GetWorldTranslate(),
		sphereCol->GetSphere().radius
	};

	Vector3 closestPoint = {
		std::clamp(secondSphere.center.x, firstAABB.min.x, firstAABB.max.x),
		std::clamp(secondSphere.center.y, firstAABB.min.y, firstAABB.max.y),
		std::clamp(secondSphere.center.z, firstAABB.min.z, firstAABB.max.z)
	};

	float distance = Length(closestPoint - secondSphere.center);

	if (distance <= secondSphere.radius) {

		return true;
	}

	return false;
}
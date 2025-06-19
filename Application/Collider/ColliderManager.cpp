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

void ColliderManager::RemoveCollider(Collider* collider) {

	auto it = std::find(colliders_.begin(), colliders_.end(), collider);

	if (it != colliders_.end()) {
		colliders_.erase(it);
	}
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

				(*it1)->SetHitTag((*it2)->GetTag());
				(*it2)->SetHitTag((*it1)->GetTag());
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

	AABB firstAABB = firstAABBCol->GetAABB();

	AABB secondAABB = secondAABBCol->GetAABB();

	Vector3 firstWorldTranslate = firstAABBCol->GetWorldTransform().GetWorldTranslate();

	Vector3 secondWorldTranslate = secondAABBCol->GetWorldTransform().GetWorldTranslate();

	firstAABB = {
		firstAABB.min + firstWorldTranslate,
		firstAABB.max + firstWorldTranslate
	};

	secondAABB = {
		secondAABB.min + secondWorldTranslate,
		secondAABB.max + secondWorldTranslate
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

	Sphere firstSphere = firstSphereCol->GetSphere();

	Sphere secondSphere = secondSphereCol->GetSphere();

	Vector3 firstWorldTranslate = firstSphereCol->GetWorldTransform().GetWorldTranslate();

	Vector3 secondWorldTranslate = secondSphereCol->GetWorldTransform().GetWorldTranslate();

	firstSphere.center += firstWorldTranslate;

	secondSphere.center += secondWorldTranslate;

	float distance = Length(firstSphere.center - secondSphere.center);

	if (distance <= firstSphere.radius + secondSphere.radius) {
		return true;
	}

	return false;
}

bool ColliderManager::IsCollisionAABBWithSphere(Collider* aabb, Collider* sphere) {

	AABBCollider* aabbCol = dynamic_cast<AABBCollider*>(aabb);
	SphereCollider* sphereCol = dynamic_cast<SphereCollider*>(sphere);

	AABB firstAABB = aabbCol->GetAABB();

	Sphere secondSphere = sphereCol->GetSphere();

	Vector3 firstWorldTranslate = aabbCol->GetWorldTransform().GetWorldTranslate();

	Vector3 secondWorldTranslate = sphereCol->GetWorldTransform().GetWorldTranslate();

	firstAABB = {
		firstAABB.min + firstWorldTranslate,
		firstAABB.max + firstWorldTranslate
	};

	secondSphere.center += secondWorldTranslate;

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
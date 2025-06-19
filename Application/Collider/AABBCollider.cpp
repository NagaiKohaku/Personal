#include "AABBCollider.h"

#include "ColliderManager.h"

#include "cstdlib"

void AABBCollider::Initialize(WorldTransform* parent) {

	Collider::Initialize(parent);

	// AABBの初期化
	aabb_.min = { -1.0f, -1.0f, -1.0f };
	aabb_.max = { 1.0f, 1.0f, 1.0f };

	worldTransform_.translate_ = {
		(aabb_.min.x + aabb_.max.x) / 2.0f,
		(aabb_.min.y + aabb_.max.y) / 2.0f,
		(aabb_.min.z + aabb_.max.z) / 2.0f
	};

	worldTransform_.scale_ = {
		(std::fabsf(aabb_.min.x) + std::fabsf(aabb_.max.x)) / 2.0f,
		(std::fabsf(aabb_.min.y) + std::fabsf(aabb_.max.y)) / 2.0f,
		(std::fabsf(aabb_.min.z) + std::fabsf(aabb_.max.z)) / 2.0f
	};

	ColliderManager::GetInstance()->AddCollider(this);
}

void AABBCollider::Update() {

	Collider::Update();
}

void AABBCollider::Draw() {

	Collider::Draw();
}

#include "AABBCollider.h"

#include "cstdlib"

void AABBCollider::Initialize(WorldTransform& parent) {

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
}

void AABBCollider::Update() {

	Collider::Update();
}

void AABBCollider::Draw() {

	Collider::Draw();
}

void AABBCollider::CreateDebugLines() {

	const Vector3 center = worldTransform_.translate_;

	const float h = 1.0f; // half-size

	for (int axis = 0; axis < 3; ++axis) {

		for (int i = -1; i <= 1; i += 2) {

			for (int j = -1; j <= 1; j += 2) {

				Vector3 start = { 0 }, end = { 0 };

				// 固定2軸の設定
				if (axis == 0) {

					// X軸方向のライン

					start = center + Vector3(-h, i * h, j * h);
					end = center + Vector3(h, i * h, j * h);

				} else if (axis == 1) {

					// Y軸方向のライン

					start = center + Vector3(i * h, -h, j * h);
					end = center + Vector3(i * h, h, j * h);

				} else {

					// Z軸方向のライン

					start = center + Vector3(i * h, j * h, -h);
					end = center + Vector3(i * h, j * h, h);
				}

				std::unique_ptr<DebugLine> line = std::make_unique<DebugLine>();

				line->Initialize(start, end, defaultColor_);

				line->SetParent(&worldTransform_);

				debugLines_.push_back(std::move(line));
			}
		}
	}
}
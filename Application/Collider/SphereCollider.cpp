#include "SphereCollider.h"

#include "numbers"

void SphereCollider::Initialize(WorldTransform& parent) {

	Collider::Initialize(parent);

	//球の初期化
	sphere_.center = { 0.0f, 0.0f, 0.0f };
	sphere_.radius = 1.0f; // 半径1の球

	worldTransform_.translate_ = sphere_.center;
	worldTransform_.scale_ = { sphere_.radius, sphere_.radius, sphere_.radius };
}

void SphereCollider::Update() {

	Collider::Update();

}

void SphereCollider::Draw() {

	Collider::Draw();

}

void SphereCollider::CreateDebugLines() {

	// 球の分割数
	const uint32_t kSubdivisions = 4;

	// 経度の間隔
	const float kLonEvery = 2.0f * static_cast<float>(std::numbers::pi) / kSubdivisions;

	// 緯度の間隔
	const float kLatEvery = static_cast<float>(std::numbers::pi) / kSubdivisions;

	for (uint32_t latIndex = 0; latIndex < kSubdivisions; ++latIndex) {

		float lat = static_cast<float>(-std::numbers::pi) / 2.0f + kLatEvery * latIndex;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivisions; ++lonIndex) {

			float lon = lonIndex * kLonEvery;

			// 球の表面上の点を計算
			Vector3 point1 = {
				std::cos(lat) * std::cos(lon),
				std::sin(lat),
				std::cos(lat) * std::sin(lon)
			};
			Vector3 point2 = {
				std::cos(lat + kLatEvery) * std::cos(lon),
				std::sin(lat + kLatEvery),
				std::cos(lat + kLatEvery) * std::sin(lon)
			};
			Vector3 point3 = {
				std::cos(lat) * std::cos(lon + kLonEvery),
				std::sin(lat),
				std::cos(lat) * std::sin(lon + kLonEvery)
			};

			// デバッグラインを生成
			std::unique_ptr<DebugLine> line1 = std::make_unique<DebugLine>();
			line1->Initialize(point1, point2, defaultColor_);
			line1->SetParent(&worldTransform_);
			debugLines_.push_back(std::move(line1));

			std::unique_ptr<DebugLine> line2 = std::make_unique<DebugLine>();
			line2->Initialize(point1, point3, defaultColor_);
			line2->SetParent(&worldTransform_);
			debugLines_.push_back(std::move(line2));
		}
	}
}
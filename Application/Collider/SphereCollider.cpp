#include "SphereCollider.h"

#include "3d/Model/ModelManager.h"
#include "ColliderManager.h"

#include "numbers"

void SphereCollider::Initialize(WorldTransform* parent) {

	Collider::Initialize(parent);

	//球の初期化
	sphere_.center = { 0.0f, 0.0f, 0.0f };
	sphere_.radius = 1.0f; // 半径1の球

	worldTransform_.translate_ = sphere_.center;
	worldTransform_.scale_ = { sphere_.radius, sphere_.radius, sphere_.radius };

	ModelManager::GetInstance()->CreatePrimitiveModel("SpherePrimitive", PrimitiveType::BALL, "Resource/Texture/white_128x128.png");

	debugObject_->SetModel("SpherePrimitive");

	debugObject_->GetWorldTransform().SetParent(&worldTransform_);

	ColliderManager::GetInstance()->AddCollider(this);
}

void SphereCollider::Update() {

	worldTransform_.translate_ = sphere_.center;
	worldTransform_.scale_ = { sphere_.radius, sphere_.radius, sphere_.radius };

	Collider::Update();
}

void SphereCollider::Draw() {

	Collider::Draw();
}

void SphereCollider::Remove() {

	ColliderManager::GetInstance()->RemoveCollider(this);
}
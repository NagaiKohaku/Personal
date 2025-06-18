#include "Collider.h"

#include "Math/MakeMatrixMath.h"

void Collider::Initialize(WorldTransform* parent) {

	worldTransform_.Initialize();

	parentTransform_ = parent;

	isDraw_ = true;

	isActive_ = true;

	defaultColor_ = { 1.0f,1.0f,1.0f,1.0f };

	hitColor_ = { 0.0f,1.0f,0.0f,1.0f };

	debugObject_ = std::make_unique<DebugObject3D>();

	debugObject_->Initialize();

}

void Collider::Update() {

	worldTransform_.UpdateMatrix();

	Matrix4x4 parentTransform = parentTransform_->ExtractTranslateMatrix();

	Matrix4x4 parentRotate = parentTransform_->ExtractRotateMatrix();

	Matrix4x4 parentScale = MakeIdentity4x4();

	Matrix4x4 parentWorldMatrix = (parentScale * parentRotate) * parentTransform;

	worldTransform_.SetWorldMatrix(worldTransform_.GetWorldMatrix() * parentWorldMatrix);

	debugObject_->Update();

	if (hitTag_ != NONE) {
		isCollision_ = true;
	} else {
		isCollision_ = false;
	}

	if (hitTag_ != hitTagBefore_) {
		isTrigger_ = true;
	} else {
		isTrigger_ = false;
	}

	hitTagBefore_ = hitTag_;

	if (isCollision_) {
		debugObject_->GetModel()->SetColor(hitColor_);
	} else {
		debugObject_->GetModel()->SetColor(defaultColor_);
	}

	hitTag_ = NONE;
}

void Collider::Draw() {

	if (!isDraw_) {
		return;
	}
	debugObject_->Draw();
}

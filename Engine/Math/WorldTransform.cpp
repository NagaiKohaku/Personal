#include "WorldTransform.h"

#include "Math/MakeMatrixMath.h"

#include "imgui.h"

#include "cmath"

void WorldTransform::Initialize() {

	translate_ = { 0.0f,0.0f,0.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	scale_ = { 1.0f,1.0f,1.0f };

	offset_ = { 0.0f,0.0f,0.0f };

	parent_ = nullptr;

	translateMatrix_ = MakeTranslateMatrix(translate_ + offset_);

	rotateMatrix_ = MakeRotateMatrix(rotate_);

	scaleMatrix_ = MakeScaleMatrix(scale_);

	localMatrix_ = (scaleMatrix_ * rotateMatrix_) * translateMatrix_;

	worldMatrix_ = localMatrix_;

	UpdateMatrix();
}

void WorldTransform::UpdateMatrix() {

	translateMatrix_ = MakeTranslateMatrix(translate_ + offset_);

	rotateMatrix_ = MakeRotateMatrix(rotate_);

	scaleMatrix_ = MakeScaleMatrix(scale_);

	localMatrix_ = (scaleMatrix_ * rotateMatrix_) * translateMatrix_;

	Matrix4x4 parentMatrix = MakeIdentity4x4();

	if (parent_) {

		Matrix4x4 pTranslateMatrix = parent_->GetWorldTranslateMatrix();
		Matrix4x4 pRotateMatrix = parent_->GetWorldRotateMatrix();
		Matrix4x4 pScaleMatrix = parent_->GetWorldScaleMatrix();

		parentMatrix = (pScaleMatrix * pRotateMatrix) * pTranslateMatrix;
	}

	worldMatrix_ = localMatrix_ * parentMatrix;
}

void WorldTransform::DisplayImGui() {

	if (ImGui::TreeNode("WorldTransform")) {

		Vector3 up = GetUp();
		Vector3 forward = GetForward();
		Vector3 right = GetRight();

		ImGui::DragFloat3("translate", &translate_.x, 0.1f);
		ImGui::DragFloat3("rotate", &rotate_.x, 0.01f);
		ImGui::DragFloat3("scale", &scale_.x, 0.1f);

		ImGui::TreePop();
	}

}

const Vector3 WorldTransform::GetForward() const {

	Vector3 result = { worldMatrix_.m[2][0],worldMatrix_.m[2][1],worldMatrix_.m[2][2] };

	return result;
}

const Vector3 WorldTransform::GetUp() const {

	Vector3 result = { worldMatrix_.m[1][0],worldMatrix_.m[1][1],worldMatrix_.m[1][2] };

	return result;
}

const Vector3 WorldTransform::GetRight() const {

	Vector3 result = { worldMatrix_.m[0][0],worldMatrix_.m[0][1],worldMatrix_.m[0][2] };

	return result;
}

const Vector3 WorldTransform::GetWorldTranslate() const {

	Vector3 result;

	result.x = worldMatrix_.m[3][0];
	result.y = worldMatrix_.m[3][1];
	result.z = worldMatrix_.m[3][2];

	return result;
}

const Vector3 WorldTransform::GetWorldRotate() const {

	Vector3 result;

	result.x = std::atan2f(worldMatrix_.m[2][1], worldMatrix_.m[2][2]);
	result.y = std::atan2f(-worldMatrix_.m[2][0], std::sqrtf(worldMatrix_.m[2][1] * worldMatrix_.m[2][1] + worldMatrix_.m[2][2] * worldMatrix_.m[2][2]));
	result.z = std::atan2f(worldMatrix_.m[1][0], worldMatrix_.m[0][0]);

	return result;
}

const Vector3 WorldTransform::GetWorldScale() const {

	Vector3 result;

	result.x = std::sqrtf(
		worldMatrix_.m[0][0] * worldMatrix_.m[0][0] +
		worldMatrix_.m[1][0] * worldMatrix_.m[1][0] +
		worldMatrix_.m[2][0] * worldMatrix_.m[2][0]
	);
	result.y = std::sqrtf(
		worldMatrix_.m[0][1] * worldMatrix_.m[0][1] +
		worldMatrix_.m[1][1] * worldMatrix_.m[1][1] +
		worldMatrix_.m[2][1] * worldMatrix_.m[2][1]
	);
	result.z = std::sqrtf(
		worldMatrix_.m[0][2] * worldMatrix_.m[0][2] +
		worldMatrix_.m[1][2] * worldMatrix_.m[1][2] +
		worldMatrix_.m[2][2] * worldMatrix_.m[2][2]
	);

	return result;
}

const Matrix4x4 WorldTransform::GetLocalTranslateMatrix() {

	return MakeTranslateMatrix(translate_ + offset_);
}

const Matrix4x4 WorldTransform::GetLocalRotateMatrix() {

	return MakeRotateMatrix(rotate_);
}

const Matrix4x4 WorldTransform::GetLocalScaleMatrix() {

	return MakeScaleMatrix(scale_);
}

const Matrix4x4 WorldTransform::GetWorldTranslateMatrix() const {

	Matrix4x4 result = MakeIdentity4x4();

	result.m[3][0] = worldMatrix_.m[3][0];
	result.m[3][1] = worldMatrix_.m[3][1];
	result.m[3][2] = worldMatrix_.m[3][2];

	return result;
}

const Matrix4x4 WorldTransform::GetWorldRotateMatrix() const {

	Matrix4x4 result = MakeIdentity4x4();

	float scaleX = std::sqrtf(
		worldMatrix_.m[0][0] * worldMatrix_.m[0][0] + 
		worldMatrix_.m[1][0] * worldMatrix_.m[1][0] + 
		worldMatrix_.m[2][0] * worldMatrix_.m[1][0]
	);

	float scaleY = std::sqrtf(
		worldMatrix_.m[0][1] * worldMatrix_.m[0][1] +
		worldMatrix_.m[1][1] * worldMatrix_.m[1][1] +
		worldMatrix_.m[2][1] * worldMatrix_.m[2][1]
	);

	float scaleZ = std::sqrtf(
		worldMatrix_.m[0][2] * worldMatrix_.m[0][2] +
		worldMatrix_.m[1][2] * worldMatrix_.m[1][2] +
		worldMatrix_.m[2][2] * worldMatrix_.m[2][2]
	);

	if (scaleX == 0.0f || scaleY == 0.0f || scaleZ == 0.0f) {
		return result;
	}

	for (int row = 0; row < 3; ++row) {

		result.m[row][0] = worldMatrix_.m[row][0] / scaleX;
		result.m[row][1] = worldMatrix_.m[row][1] / scaleY;
		result.m[row][2] = worldMatrix_.m[row][2] / scaleZ;
	}

	return result;
}

const Matrix4x4 WorldTransform::GetWorldScaleMatrix() const {

	Matrix4x4 result = MakeIdentity4x4();

	result.m[0][0] = std::sqrtf(
		worldMatrix_.m[0][0] * worldMatrix_.m[0][0] +
		worldMatrix_.m[1][0] * worldMatrix_.m[1][0] +
		worldMatrix_.m[2][0] * worldMatrix_.m[2][0]
	);

	result.m[1][1] = std::sqrtf(
		worldMatrix_.m[0][1] * worldMatrix_.m[0][1] +
		worldMatrix_.m[1][1] * worldMatrix_.m[1][1] +
		worldMatrix_.m[2][1] * worldMatrix_.m[2][1]
	);

	result.m[2][2] = std::sqrtf(
		worldMatrix_.m[0][2] * worldMatrix_.m[0][2] +
		worldMatrix_.m[1][2] * worldMatrix_.m[1][2] +
		worldMatrix_.m[2][2] * worldMatrix_.m[2][2]
	);

	return result;
}

void WorldTransform::SetParent(WorldTransform* parent) {

	this->parent_ = parent;
}
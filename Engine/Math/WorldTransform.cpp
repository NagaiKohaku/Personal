#include "WorldTransform.h"

#include "Math/MakeMatrixMath.h"

#include "imgui.h"

#include "cmath"

void WorldTransform::Initialize() {

	translate_ = { 0.0f,0.0f,0.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	scale_ = { 1.0f,1.0f,1.0f };

	offset_ = { 0.0f,0.0f,0.0f };

	UpdateMatrix();
}

void WorldTransform::UpdateMatrix() {

	translateMatrix_ = MakeTranslateMatrix(translate_ + offset_);

	rotateMatrix_ = MakeRotateMatrix(rotate_);

	scaleMatrix_ = MakeScaleMatrix(scale_);

	if (parent_) {

		Matrix4x4 pTranslateMatrix = parent_->ExtractTranslateMatrix();
		Matrix4x4 pRotateMatrix = parent_->ExtractRotateMatrix();
		Matrix4x4 pScaleMatrix = parent_->ExtractScaleMatrix();

		translateMatrix_ *= pTranslateMatrix;

		rotateMatrix_ *= pRotateMatrix;

		scaleMatrix_ *= pScaleMatrix;
	}

	worldMatrix_ = (scaleMatrix_ * rotateMatrix_) * translateMatrix_;
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

const Vector3& WorldTransform::GetForward() const {

	Vector3 result = { worldMatrix_.m[2][0],worldMatrix_.m[2][1],worldMatrix_.m[2][2] };

	return result;
}

const Vector3& WorldTransform::GetUp() const {

	Vector3 result = { worldMatrix_.m[1][0],worldMatrix_.m[1][1],worldMatrix_.m[1][2] };

	return result;
}

const Vector3& WorldTransform::GetRight() const {

	Vector3 result = { worldMatrix_.m[0][0],worldMatrix_.m[0][1],worldMatrix_.m[0][2] };

	return result;
}

const Matrix4x4& WorldTransform::GetTranslateMatrix() {

	return MakeTranslateMatrix(translate_ + offset_);
}

const Matrix4x4& WorldTransform::GetRotateMatrix() {

	return MakeRotateMatrix(rotate_);
}

const Matrix4x4& WorldTransform::GetScaleMatrix() {

	return MakeScaleMatrix(scale_);
}

const Matrix4x4& WorldTransform::ExtractTranslateMatrix() const {

	Matrix4x4 result = MakeIdentity4x4();

	result.m[3][0] = worldMatrix_.m[3][0];
	result.m[3][1] = worldMatrix_.m[3][1];
	result.m[3][2] = worldMatrix_.m[3][2];

	return result;
}

const Matrix4x4& WorldTransform::ExtractRotateMatrix() const {

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

const Matrix4x4& WorldTransform::ExtractScaleMatrix() const {

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

const Vector3& WorldTransform::GetWorldTranslate() const {

	Vector3 result;

	result.x = worldMatrix_.m[3][0];
	result.y = worldMatrix_.m[3][1];
	result.z = worldMatrix_.m[3][2];

	return result;
}

void WorldTransform::SetParent(WorldTransform* parent) {

	this->parent_ = parent;
}
#include "WorldTransform.h"

#include "Math/MakeMatrixMath.h"

#include "imgui.h"

void WorldTransform::Initialize() {

	translate_ = { 0.0f,0.0f,0.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	scale_ = { 1.0f,1.0f,1.0f };

	offset_ = { 0.0f,0.0f,0.0f };

	UpdateMatrix();
}

void WorldTransform::UpdateMatrix() {

	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_ + offset_);

	if (parent_) {

		worldMatrix_ *= parent_->GetWorldMatrix();
	}
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
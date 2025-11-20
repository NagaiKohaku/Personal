#include "Building.h"

#include "3d/Object/Object3D.h"

#include "3d/Model/Model.h"

void Building::Initialize(Vector3 pos, Vector3 scale) {

	object_ = std::make_unique<Object3D>();

	object_->Initialize();

	object_->SetModel("Cube");

	object_->GetWorldTransform().scale_ = scale;

	object_->GetWorldTransform().translate_ = pos;

	speed_ = 200.0f;
}

void Building::Update() {

	object_->Update();

	object_->GetWorldTransform().translate_.z -= speed_ * 1.0f / 60.0f;
}

void Building::TransformUpdate() {

	object_->Update();
}

void Building::Draw() {

	object_->Draw(LayerType::Object);
}

WorldTransform Building::GetWorldTransform() {
	return object_->GetWorldTransform();
}

void Building::SetPosition(Vector3 pos) {
	object_->GetWorldTransform().translate_ = pos;
}

void Building::SetPosZ(float z) {
	object_->GetWorldTransform().translate_.z = z;
}
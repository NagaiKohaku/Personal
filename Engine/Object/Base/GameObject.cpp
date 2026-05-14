#include "GameObject.h"

#include "Base/LayerType.h"

using namespace MyEngine;

void GameObject::Initialize(Camera* cameraPtr, std::string name) {

	camera_ = cameraPtr;

	SetName(name);

	CreateWorldTransform();
}

void GameObject::Update() {

	Matrix4x4 viewProjectionMatrix = camera_->Get3DViewProjectionMatrix();

	for (auto& transform : partTransforms_) {
		transform->UpdateMatrix(viewProjectionMatrix);
	}
}

void GameObject::Draw() {

}

void MyEngine::GameObject::CreateWorldTransform() {

	std::unique_ptr<WorldTransform> newTransform;

	newTransform = std::make_unique<WorldTransform>();

	newTransform->Initialize();

	partTransforms_.push_back(std::move(newTransform));
}

ComponentBase* MyEngine::GameObject::GetComponent() {
	return nullptr;
}
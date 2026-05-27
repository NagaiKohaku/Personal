#include "GameObject.h"

#include "Base/LayerType.h"

using namespace MyEngine;

void GameObject::Initialize(DirectXCommon* directCommonPtr, std::string name) {

	SetName(name);

	CreateWorldTransform(directCommonPtr);
}

void GameObject::Update() {

	for (auto& transform : partTransforms_) {
		transform->Update();
	}

	for (auto& component : components_) {
		component->Update();
	}
}

void GameObject::Draw() {

	for (auto& transform : partTransforms_) {
		transform->Draw();
	}

	for (auto& component : components_) {
		component->Draw();
	}
}

void MyEngine::GameObject::CreateWorldTransform(DirectXCommon* directCommonPtr) {

	std::unique_ptr<WorldTransform> newTransform;

	newTransform = std::make_unique<WorldTransform>();

	newTransform->SetDirectCommon(directCommonPtr);

	newTransform->Initialize();

	partTransforms_.push_back(std::move(newTransform));
}

ComponentBase* MyEngine::GameObject::GetComponent() {
	return nullptr;
}
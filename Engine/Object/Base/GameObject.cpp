#include "GameObject.h"

#include "Base/LayerType.h"

using namespace MyEngine;

void GameObject::Initialize() {

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
		transform->SendDataForGPU();
	}

	for (auto& component : components_) {
		component->SendDataForGPU();
	}
}
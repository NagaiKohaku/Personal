#include "ComponentManager.h"

#include <Object/3D/Model/ModelManager.h>

using namespace MyEngine;

ComponentManager* ComponentManager::GetInstance() {
	static ComponentManager instance;
	return &instance;
}

void ComponentManager::Initialize(DirectXCommon* directCommonPtr) {

	directCommon_ = directCommonPtr;
}

std::shared_ptr<Model> MyEngine::ComponentManager::CreateModelComponent(const std::string& fileName) {

	return ModelManager::GetInstance()->GetModel(fileName);
}

std::shared_ptr<Sprite> MyEngine::ComponentManager::CreateSpriteComponent() {
	return std::unique_ptr<Sprite>();
}

std::shared_ptr<WorldTransform> MyEngine::ComponentManager::CreateWorldTransformComponent() {

	std::shared_ptr<WorldTransform> newWorldTransform = std::make_unique<WorldTransform>();

	newWorldTransform->Initialize();

	return newWorldTransform;
}
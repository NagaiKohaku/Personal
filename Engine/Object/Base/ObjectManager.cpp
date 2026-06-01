#include "ObjectManager.h"

using namespace MyEngine;

void ObjectManager::Initialize(DirectXCommon* directCommonPtr) {

	directCommon_ = directCommonPtr;
}

void ObjectManager::Update() {

	for (auto& object : gameObjects_) {
		object.second->Update();
	}
}

void ObjectManager::Draw() {

	for (auto& object : gameObjects_) {
		object.second->Draw();
	}
}

void ObjectManager::CreateGameObject(const std::string& name) {

	std::string baseName = name;
	std::string newName = baseName;
	int number = 1;

	while (gameObjects_.contains(newName)) {
		newName = name + std::to_string(number);
		number++;
	}

	gameObjects_[newName] = std::make_unique<GameObject>();

	gameObjects_[newName]->Initialize();
}

GameObject* ObjectManager::GetGameObject(const std::string& name) {

	for (auto& object : gameObjects_) {

		if (object.first == name) {

			return object.second.get();
		}
	}

	return nullptr;
}
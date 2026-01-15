#include "UIManager.h"

#include <2d/Sprite/SpriteManager.h>

UIManager* UIManager::GetInstance() {
	static UIManager instance;
	return &instance;
}

void UIManager::Initialize() {

}

void UIManager::Update() {
	for (auto& [groupName, uiObjects] : ui) {
		for (auto& uiObject : uiObjects) {
			uiObject->Update();
		}
	}
}

void UIManager::Draw() {
	for (auto& [groupName, uiObjects] : ui) {
		for (auto& uiObject : uiObjects) {
			uiObject->Draw(LayerType::UI);
		}
	}
}

void UIManager::CreateUI(const std::string& groupName, const std::string& spriteName) {
	auto uiObject = std::make_unique<Object2D>();
	uiObject->Initialize();
	uiObject->SetSprite(spriteName);
	ui[groupName].push_back(std::move(uiObject));
}
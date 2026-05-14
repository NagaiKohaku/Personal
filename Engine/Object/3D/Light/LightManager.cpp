#include "LightManager.h"

using namespace MyEngine;

void LightManager::Initialize(DirectXCommon* directCommonPtr) {

	directionalLight_ = std::make_unique<DirectionalLight>();

	directionalLight_->Initialize(directCommonPtr);
}

void LightManager::Update() {

	directionalLight_->Update();
}

void LightManager::Draw() {

	directionalLight_->SendDataForGPU();
}
#include "CameraManager.h"

using namespace MyEngine;

CameraManager* CameraManager::GetInstance() {
	static CameraManager instance;
	return &instance;
}

void CameraManager::Initialize(DirectXCommon* directCommonPtr, WinApp* winAppPtr, Input* inputPtr) {

	directCommon_ = directCommonPtr;

	winApp_ = winAppPtr;

	input_ = inputPtr;

	CreateCamera();

	mainCamera_ = cameras_[0].get();

}

void CameraManager::Update() {

	mainCamera_->Update();
}

void CameraManager::Draw() {

	mainCamera_->SendDataForGPU();
}

void CameraManager::CreateCamera() {

	std::unique_ptr<Camera> newCamera = std::make_unique<Camera>();

	newCamera->Initialize(winApp_->GetWindowWidth(), winApp_->GetWindowHeight(), directCommon_, input_);

	cameras_.push_back(std::move(newCamera));
}
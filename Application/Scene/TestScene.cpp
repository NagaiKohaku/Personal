#include "TestScene.h"

using namespace MyEngine;

void TestScene::Initialize(EngineContext context) {

	BaseScene::Initialize(context);

	//デバッグカメラを使用しない
	context_.cameraManager->GetMainCamera()->SetDebugCameraFlag(false);

	//カメラの座標
	context_.cameraManager->GetMainCamera()->GetWorldTransform().translate_ = { 0.0f,1.0f,0.0f };

	context_.cameraManager->GetMainCamera()->SetOffsetZ(-20.0f);

	gameObject_ = std::make_unique<GameObject>();

	gameObject_->Initialize()
}

void TestScene::Finalize() {

}

void TestScene::Update() {

}

void TestScene::Draw() {

}

void TestScene::ImGui() {

#ifdef _USE_IMGUI

#endif // _USE_IMGUI

}
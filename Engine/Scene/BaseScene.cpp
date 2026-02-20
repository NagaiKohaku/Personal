#include "BaseScene.h"

namespace MyEngine {

	void BaseScene::Initialize(Camera* cameraPtr) {

		camera_ = cameraPtr;
	}

	void BaseScene::Finalize() {
	}

	void BaseScene::Update() {
	}

	void BaseScene::Draw() {
	}

	void BaseScene::ImGui() {
	}
}
#pragma once

#include <Object/3D/Camera/Camera.h>
#include <Base/DirectXCommon.h>
#include <Base/WinApp.h>
#include <Base/Input.h>

#include <vector>
#include <memory>

namespace MyEngine {

	class CameraManager {

	public:

		static CameraManager* GetInstance();

		void Initialize(DirectXCommon* directCommonPtr, WinApp* winAppPtr, Input* inputPtr);

		void Update();

		void Draw();

	private:

		void CreateCamera();

	private:

		DirectXCommon* directCommon_;

		WinApp* winApp_;

		Input* input_;

		Camera* mainCamera_;

		std::vector<std::unique_ptr<Camera>> cameras_;

	public:

		Camera* GetMainCamera() { return mainCamera_; }
	};
}
#pragma once

#include <Base/DirectXCommon.h>

#include <Object/3D/Light/DirectionalLight/DirectionalLight.h>

#include <memory>

namespace MyEngine {

	class LightManager {

	public:

		void Initialize(DirectXCommon* directCommonPtr);

		void Update();

		void Draw();

	private:

		std::unique_ptr<DirectionalLight> directionalLight_;
	};
}
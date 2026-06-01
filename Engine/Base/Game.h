#pragma once

#include <Base/Framework.h>

#include <Base/ImGuiManager.h>
#include <Object/2D/Sprite/SpriteManager.h>
#include <Object/3D/Model/ModelManager.h>
#include <Object/Base/ObjectManager.h>
#include <Object/3D/Camera/CameraManager.h>
#include <LevelEditor/LevelDataLoader.h>
#include <Scene/EngineContext.h>
#include <Scene/SceneManager.h>

#include <memory>

namespace MyEngine {

	class Game : public Framework {

	public:

		void Initialize() override;

		void Update() override;

		void Draw() override;

		void Finalize() override;

	private:

		std::unique_ptr<SpriteManager> spriteManager_;

		std::unique_ptr<ObjectManager> objectManager_;

		CameraManager* cameraManager_;

		std::unique_ptr<LevelDataLoader> levelDataLoder_;

		std::unique_ptr<SceneManager> sceneManager_;

		EngineContext engineContext_;

	};
}
#pragma once

#include <Base/Framework.h>

#include <Base/ImGuiManager.h>
#include <Object/2D/Sprite/SpriteManager.h>
#include <Object/3D/Model/ModelManager.h>
#include <Object/Base/ObjectManager.h>
#include <Object/3D/Camera/CameraManager.h>
#include <3d/Collider/ColliderManager.h>
#include <Effect/Manager/EmitterManager.h>
#include <Effect/Manager/UIManager.h>
#include <LevelEditor/LevelDataLoader.h>
#include <Scene/EngineContext.h>
#include <Scene/SceneManager.h>
#include <Effect/Fade/Fade.h>
#include <Effect/Flash/Flash.h>

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

		std::unique_ptr<CameraManager> cameraManager_;

		std::unique_ptr<ColliderManager> colliderManager_;

		std::unique_ptr<EmitterManager> emitterManager_;

		std::unique_ptr<UIManager> uiManager_;

		std::unique_ptr<LevelDataLoader> levelDataLoder_;

		std::unique_ptr<SceneManager> sceneManager_;

		EngineContext engineContext_;

		Fade* fade_;

		Flash* flash_;
	};
}
#pragma once

#include <Base/Framework.h>

#include <Base/ImGuiManager.h>
#include <2d/Sprite/SpriteManager.h>
#include <3d/Collider/ColliderManager.h>
#include <Object/Manager/ObjectManager.h>
#include <Effect/Manager/EmitterManager.h>
#include <UI/UIManager.h>
#include <LevelEditor/LevelDataLoader.h>
#include <3d/Camera/Camera.h>
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

		SpriteManager* spriteManager_;

		ColliderManager* colliderManager_;

		ObjectManager* objectManager_;

		EmitterManager* emitterManager_;

		UIManager* uiManager_;

		LevelDataLoader* levelDataLoder_;

		std::unique_ptr<Camera> camera_;

		EngineContext engineContext_;

		SceneManager* sceneManager_;

		Fade* fade_;

		Flash* flash_;
	};
}
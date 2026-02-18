#pragma once

#include <Base/Framework.h>

#include <Base/ImGuiManager.h>
#include <Object/Manager/ObjectManager.h>
#include <Effect/Manager/EmitterManager.h>
#include <Effect/Manager/UIManager.h>
#include <2d/Sprite/SpriteManager.h>
#include <3d/Collider/ColliderManager.h>
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

		SceneManager* sceneManager_;

		Fade* fade_;

		Flash* flash_;
	};
}
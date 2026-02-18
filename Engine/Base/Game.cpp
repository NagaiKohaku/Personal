#include "Game.h"

namespace MyEngine {

	void Game::Initialize() {

		Framework::Initialize();

		spriteManager_ = SpriteManager::GetInstance();
		spriteManager_->Initialize();

		colliderManager_ = ColliderManager::GetInstance();
		colliderManager_->Initialize();

		objectManager_ = ObjectManager::GetInstance();
		objectManager_->Initialize();

		emitterManager_ = EmitterManager::GetInstance();
		emitterManager_->Initialize();

		uiManager_ = UIManager::GetInstance();
		uiManager_->Initialize();

		sceneManager_ = SceneManager::GetInstance();
		sceneManager_->Initialize();

		fade_ = Fade::GetInstance();
		fade_->Initialize();

		flash_ = Flash::GetInstance();
		flash_->Initialize();

		sceneManager_->ChangeScene(SceneManager::SceneType::kTitle);

	}

	void Game::Update() {

		Framework::BeginImGui();

		Framework::Update();

		objectManager_->Update();

		emitterManager_->Update();

		uiManager_->Update();

		colliderManager_->Update();

		sceneManager_->Update();

		fade_->Update();

		flash_->Update();

#ifdef _USE_IMGUI

		uiManager_->ImGui();

		sceneManager_->ImGui();

#endif // _USE_IMGUI

		Framework::EndImGui();

	}

	void Game::Draw() {

		Framework::PreDraw();

		objectManager_->Draw();

		emitterManager_->Draw();

		uiManager_->Draw();

		fade_->Draw();

		flash_->Draw();

		Framework::PostDraw();
	}

	void Game::Finalize() {

		Framework::Finalize();

	}
}
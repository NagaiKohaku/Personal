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

		camera_ = std::make_unique<Camera>();
		camera_->Initialize(static_cast<float>(Framework::GetWinApp()->GetWindowWidth()), static_cast<float>(Framework::GetWinApp()->GetWindowHeight()));

		emitterManager_ = EmitterManager::GetInstance();
		emitterManager_->Initialize();

		uiManager_ = UIManager::GetInstance();
		uiManager_->Initialize(camera_.get());

		levelDataLoder_ = LevelDataLoader::GetInstance();
		levelDataLoder_->Initialize();

		sceneManager_ = SceneManager::GetInstance();
		sceneManager_->Initialize(camera_.get());

		fade_ = Fade::GetInstance();
		fade_->Initialize(Framework::GetWinApp(),camera_.get());

		flash_ = Flash::GetInstance();
		flash_->Initialize(camera_.get());

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

		sceneManager_->Draw();

		fade_->Draw();

		flash_->Draw();

		Framework::PostDraw();
	}

	void Game::Finalize() {

		Framework::Finalize();

	}
}
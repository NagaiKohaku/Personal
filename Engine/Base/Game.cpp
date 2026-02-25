#include "Game.h"

namespace MyEngine {

	void Game::Initialize() {

		Framework::Initialize();

		spriteManager_ = SpriteManager::GetInstance();
		spriteManager_->Initialize(spriteCommon_.get());

		colliderManager_ = ColliderManager::GetInstance();
		colliderManager_->Initialize();

		objectManager_ = ObjectManager::GetInstance();
		objectManager_->Initialize();

		camera_ = std::make_unique<Camera>();
		camera_->Initialize(
			static_cast<float>(winApp_->GetWindowWidth()),
			static_cast<float>(winApp_->GetWindowHeight()),
			offScreen_.get(),
			object3DCommon_.get(),
			debugObjectCommon_.get(),
			skyBoxCommon_.get()
		);

		emitterManager_ = EmitterManager::GetInstance();
		emitterManager_->Initialize(particleCommon_.get());

		uiManager_ = UIManager::GetInstance();
		uiManager_->Initialize(object2DCommon_.get(), camera_.get());

		levelDataLoder_ = LevelDataLoader::GetInstance();
		levelDataLoder_->Initialize();

		engineContext_.offScreen = offScreen_.get();
		engineContext_.object2DCommon = object2DCommon_.get();
		engineContext_.object3DCommon = object3DCommon_.get();
		engineContext_.particleCommon = particleCommon_.get();
		engineContext_.debugObjectCommon = debugObjectCommon_.get();
		engineContext_.camera = camera_.get();
		engineContext_.audio = audio_;
		engineContext_.input = input_;

		sceneManager_ = SceneManager::GetInstance();
		sceneManager_->Initialize(engineContext_);

		fade_ = Fade::GetInstance();
		fade_->Initialize(winApp_.get(), object2DCommon_.get(), camera_.get());

		flash_ = Flash::GetInstance();
		flash_->Initialize(object2DCommon_.get(), camera_.get());

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
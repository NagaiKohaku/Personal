#include "Game.h"

namespace MyEngine {

	void Game::Initialize() {

		Framework::Initialize();

		spriteManager_ = std::make_unique<SpriteManager>();
		spriteManager_->Initialize(dxCommon_.get());

		cameraManager_ = std::make_unique<CameraManager>();
		cameraManager_->Initialize(dxCommon_.get(), winApp_.get(), input_.get());

		objectManager_ = std::make_unique<ObjectManager>();
		objectManager_->Initialize(dxCommon_.get());

		levelDataLoder_ = std::make_unique<LevelDataLoader>();
		levelDataLoder_->Initialize();

		engineContext_.offScreen = offScreen_.get();
		engineContext_.objectManager = objectManager_.get();
		engineContext_.cameraManager = cameraManager_.get();
		engineContext_.input = input_.get();
		engineContext_.audio = audio_.get();
		engineContext_.renderer = renderer_.get();

		sceneManager_ = std::make_unique<SceneManager>();
		sceneManager_->Initialize(engineContext_);

		sceneManager_->ChangeScene(SceneManager::SceneType::kTest);

	}

	void Game::Update() {

		Framework::BeginImGui();

		Framework::Update();

		sceneManager_->Update();

#ifdef _USE_IMGUI

		sceneManager_->ImGui();

#endif // _USE_IMGUI

		Framework::EndImGui();

	}

	void Game::Draw() {

		Framework::PreDraw();

		sceneManager_->Draw();

		objectManager_->Draw();

		Framework::PostDraw();
	}

	void Game::Finalize() {

		Framework::Finalize();

	}
}
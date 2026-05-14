#include "Game.h"

namespace MyEngine {

	void Game::Initialize() {

		Framework::Initialize();

		spriteManager_ = std::make_unique<SpriteManager>();
		spriteManager_->Initialize(object2DCommon_.get());

		colliderManager_ = std::make_unique<ColliderManager>();
		colliderManager_->Initialize();

		cameraManager_ = std::make_unique<CameraManager>();
		cameraManager_->Initialize(dxCommon_.get(), winApp_.get(), input_.get());

		objectManager_ = std::make_unique<ObjectManager>();

		emitterManager_ = std::make_unique<EmitterManager>();
		emitterManager_->Initialize(particleCommon_.get(), input_.get(), renderer_.get());

		uiManager_ = std::make_unique<UIManager>();
		uiManager_->Initialize(object2DCommon_.get(), cameraManager_->GetMainCamera(), input_.get(), renderer_.get());

		levelDataLoder_ = std::make_unique<LevelDataLoader>();
		levelDataLoder_->Initialize();

		engineContext_.offScreen = offScreen_.get();
		engineContext_.objectManager = objectManager_.get();
		engineContext_.objectCommon.object2DCommon = object2DCommon_.get();
		engineContext_.objectCommon.object3DCommon = object3DCommon_.get();
		engineContext_.objectCommon.particleCommon = particleCommon_.get();
		engineContext_.objectCommon.debugObjectCommon = debugObjectCommon_.get();
		engineContext_.cameraManager = cameraManager_.get();
		engineContext_.input = input_.get();
		engineContext_.audio = audio_.get();
		engineContext_.renderer = renderer_.get();

		sceneManager_ = std::make_unique<SceneManager>();
		sceneManager_->Initialize(engineContext_);

		fade_ = Fade::GetInstance();
		fade_->Initialize(winApp_.get(), object2DCommon_.get(), cameraManager_->GetMainCamera(), renderer_.get());

		flash_ = Flash::GetInstance();
		flash_->Initialize(object2DCommon_.get(), cameraManager_->GetMainCamera(), renderer_.get());

		sceneManager_->ChangeScene(SceneManager::SceneType::kTest);

	}

	void Game::Update() {

		Framework::BeginImGui();

		Framework::Update();

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
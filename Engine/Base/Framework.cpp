#include "Framework.h"

#include <Base/WinApp.h>
#include <Base/DirectXCommon.h>
#include <Base/View/RTVManager.h>
#include <Base/View/DSVManager.h>
#include <Base/View/SRVManager.h>
#include <Base/OffScreen.h>
#include <2d/Sprite/SpriteCommon.h>
#include <3d/Model/ModelCommon.h>
#include <2d/Sprite/TextureManager.h>
#include <3d/Model/ModelManager.h>
#include <2d/Object/Object2DCommon.h>
#include <3d/Object/Object3DCommon.h>
#include <3d/Object/DebugObjectCommon.h>
#include <3d/Object/SkyBoxCommon.h>
#include <3d/Particle/ParticleCommon.h>
#include <Base/ImGuiManager.h>
#include <Base/Renderer.h>
#include <Base/Input.h>
#include <Base/Audio.h>

namespace MyEngine {

	Framework::Framework() = default;

	Framework::~Framework() = default;

	void Framework::Run() {

		// エンジンの初期化
		Initialize();

		while (true) {

			Update();

			if (IsEndRequest()) {
				break;
			}

			Draw();
		}

		Finalize();
	}

	void Framework::Initialize() {

		///-------------------------------------------/// 
		/// エンジン初期化
		///-------------------------------------------///

		//ウィンドウ
		winApp_ = std::make_unique<WinApp>();
		winApp_->Initialize();

		//DirectX基底
		dxCommon_ = std::make_unique<DirectXCommon>();
		dxCommon_->Initialize(winApp_.get());

		//RTVマネージャー
		rtvManager_ = std::make_unique<RTVManager>();
		rtvManager_->Initialize(dxCommon_.get());

		//DSVマネージャー
		dsvManager_ = std::make_unique<DSVManager>();
		dsvManager_->Initialize(dxCommon_.get());

		//SRVマネージャー
		srvManager_ = std::make_unique<SRVManager>();
		srvManager_->Initialize(dxCommon_.get());

		//描画系の初期化
		dxCommon_->InitializeRendering(rtvManager_.get(), dsvManager_.get());

		//オフスクリーン
		offScreen_ = std::make_unique<OffScreen>();
		offScreen_->Initialize(winApp_.get(), dxCommon_.get(), rtvManager_.get(), dsvManager_.get(), srvManager_.get());

		//スプライト基底
		spriteCommon_ = std::make_unique<SpriteCommon>();
		spriteCommon_->Initialize(dxCommon_.get());

		//モデル基底
		modelCommon_ = std::make_unique<ModelCommon>();
		modelCommon_->Initialize(dxCommon_.get());

		//テクスチャマネージャー
		TextureManager::GetInstance()->Initialize(dxCommon_.get(), srvManager_.get());

		//モデルマネージャー
		ModelManager::GetInstance()->Initialize(modelCommon_.get());

		//2Dオブジェクト基底
		object2DCommon_ = std::make_unique<Object2DCommon>();
		object2DCommon_->Initialize(dxCommon_.get());

		//3Dオブジェクト基底
		object3DCommon_ = std::make_unique<Object3DCommon>();
		object3DCommon_->Initialize(dxCommon_.get());

		//デバッグオブジェクト基底
		debugObjectCommon_ = std::make_unique<DebugObjectCommon>();
		debugObjectCommon_->Initialize(dxCommon_.get());

		//スカイボックス基底
		skyBoxCommon_ = std::make_unique<SkyBoxCommon>();
		skyBoxCommon_->Initialize(dxCommon_.get());

		//パーティクル基底
		particleCommon_ = std::make_unique<ParticleCommon>();
		particleCommon_->Initialize(dxCommon_.get(), srvManager_.get());

		//ImGuiマネージャー
		imGuiManager_ = std::make_unique<ImGuiManager>();
		imGuiManager_->Initialize(winApp_.get(), dxCommon_.get());

		//入力
		input_ = std::make_unique<Input>();
		input_->Initialize(winApp_.get());

		//音声
		audio_ = std::make_unique<Audio>();
		audio_->Initialize();

		//レンダラー
		renderer_ = std::make_unique<Renderer>();
		renderer_->Initialize();

	}

	void Framework::Finalize() {

		audio_->Finalize();

		imGuiManager_->Finalize();

		winApp_->Finalize();
	}

	void Framework::Update() {

		if (winApp_->ProcessMessage()) {
			endRequest_ = true;
		}

		audio_->Update();

		input_->Update();

		object3DCommon_->Update();

#ifdef _USE_IMGUI

		offScreen_->ImGui();

#endif // _USE_IMGUI

	}

	void Framework::PreDraw() {

		offScreen_->PreDraw();

		srvManager_->PreDraw();
	}

	void Framework::PostDraw() {

		renderer_->OffScreenDraw();

		offScreen_->PostDraw();

		dxCommon_->PreDraw();

		srvManager_->PreDraw();

		offScreen_->DrawToSwapChain();

		renderer_->SwapChainDraw();

#ifdef _USE_IMGUI

		//ImGuiの描画
		imGuiManager_->Draw();

#endif // _USE_IMGUI

		dxCommon_->PostDraw();
	}

	void Framework::BeginImGui() {

		imGuiManager_->Begin();
	}

	void Framework::EndImGui() {

		imGuiManager_->End();
	}

}
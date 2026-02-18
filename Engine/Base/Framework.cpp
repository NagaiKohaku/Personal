#include "Framework.h"

#include <2d/Sprite/TextureManager.h>
#include <3d/Model/ModelManager.h>

namespace MyEngine {

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
		winApp_ = WinApp::GetInstance();
		winApp_->Initialize();

		//DirectX基底
		directXCommon_ = DirectXCommon::GetInstance();
		directXCommon_->Initialize();

		//RTVマネージャー
		rtvManager_ = RTVManager::GetInstance();
		rtvManager_->Initialize();

		//DSVマネージャー
		dsvManager_ = DSVManager::GetInstance();
		dsvManager_->Initialize();

		//SRVマネージャー
		srvManager_ = SrvManager::GetInstance();
		srvManager_->Initialize();

		//描画系の初期化
		directXCommon_->InitializeRendering();

		//オフスクリーン
		offScreen_ = OffScreen::GetInstance();
		offScreen_->Initialize();

		//スプライト基底
		spriteCommon_ = SpriteCommon::GetInstance();
		spriteCommon_->Initialize();

		//モデル基底
		modelCommon_ = ModelCommon::GetInstance();
		modelCommon_->Initialize();

		//テクスチャマネージャー
		TextureManager::GetInstance()->Initialize();

		//モデルマネージャー
		ModelManager::GetInstance()->Initialize();

		//2Dオブジェクト基底
		object2DCommon_ = Object2DCommon::GetInstance();
		object2DCommon_->Initialize();

		//3Dオブジェクト基底
		object3DCommon_ = Object3DCommon::GetInstance();
		object3DCommon_->Initialize();

		//デバッグオブジェクト基底
		debugObjectCommon_ = DebugObjectCommon::GetInstance();
		debugObjectCommon_->Initialize();

		//スカイボックス基底
		skyBoxCommon_ = SkyBoxCommon::GetInstance();
		skyBoxCommon_->Initialize();

		//パーティクル基底
		particleCommon_ = ParticleCommon::GetInstance();
		particleCommon_->Initialize();

		//ImGuiマネージャー
		imGuiManager_ = ImGuiManager::GetInstance();
		imGuiManager_->Initialize();

		//入力
		input_ = Input::GetInstance();
		input_->Initialize();

		//音声
		audio_ = Audio::GetInstance();
		audio_->Initialize();

		//レンダラー
		renderer_ = Renderer::GetInstance();
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

		directXCommon_->PreDraw();

		srvManager_->PreDraw();

		offScreen_->DrawToSwapChain();

		renderer_->SwapChainDraw();

#ifdef _USE_IMGUI

		//ImGuiの描画
		imGuiManager_->Draw();

#endif // _USE_IMGUI

		directXCommon_->PostDraw();
	}

	void Framework::BeginImGui() {

		imGuiManager_->Begin();
	}

	void Framework::EndImGui() {

		imGuiManager_->End();
	}

}
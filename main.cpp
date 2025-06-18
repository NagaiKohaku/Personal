#include "Base/WinApp.h"
#include "Base/DirectXCommon.h"
#include "Base/OffScreen.h"
#include "Base/RTVManager.h"
#include "Base/DSVManager.h"
#include "Base/SrvManager.h"
#include "Base/Input.h"
#include "Base/Audio.h"
#include "Base/ImGuiManager.h"
#include "Base/Renderer.h"
#include "2d/Sprite/SpriteCommon.h"
#include "2d/Object/Object2DCommon.h"
#include "2d/Sprite/TextureManager.h"
#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"
#include "3d/Model/ModelCommon.h"
#include "3d/Model/ModelManager.h"
#include "3d/Particle/ParticleCommon.h"
#include "3d/Particle/ParticleManager.h"
#include "Application/Collider/ColliderManager.h"

#include "Scene/SceneManager.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	///-------------------------------------------/// 
	/// エンジン初期化
	///-------------------------------------------///

	//ウィンドウ
	WinApp* winApp = WinApp::GetInstance();
	winApp->Initialize();

	//DirectX基底
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	directXCommon->Initialize();

	//RTVマネージャー
	RTVManager* rtvManager = RTVManager::GetInstance();
	rtvManager->Initialize();

	//DSVマネージャー
	DSVManager* dsvManager = DSVManager::GetInstance();
	dsvManager->Initialize();

	//SRVマネージャー
	SrvManager* srvManager = SrvManager::GetInstance();
	srvManager->Initialize();

	//描画系の初期化
	directXCommon->InitializeRendering();

	//ImGuiマネージャー
	ImGuiManager* imGuiManager = ImGuiManager::GetInstance();
	imGuiManager->Initialize();

	//オフスクリーン
	OffScreen* offScreen = OffScreen::GetInstance();
	offScreen->Initialize();

	//モデル基底
	ModelCommon* modelCommon = ModelCommon::GetInstance();
	modelCommon->Initialize();

	//スプライト基底
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize();

	//3Dオブジェクト基底
	Object3DCommon* object3DCommon = Object3DCommon::GetInstance();
	object3DCommon->Initialize();

	//2Dオブジェクト基底
	Object2DCommon* object2dCommon = Object2DCommon::GetInstance();
	object2dCommon->Initialize();

	//デバッグオブジェクト基底
	DebugObjectCommon* debugObjectCommon = DebugObjectCommon::GetInstance();
	debugObjectCommon->Initialize();

	//パーティクル基底
	ParticleCommon* particleCommon = ParticleCommon::GetInstance();
	particleCommon->Initialize();

	//パーティクルマネージャー
	ParticleManager* particleManager = ParticleManager::GetInstance();
	particleManager->Initialize();

	//入力
	Input* input = Input::GetInstance();
	input->Initialize();

	//音声
	Audio* audio = Audio::GetInstance();
	audio->Initialize();

	//シーンマネージャー
	SceneManager* sceneManager = SceneManager::GetInstance();
	sceneManager->Initialize();

	//レンダラー
	Renderer* renderer = Renderer::GetInstance();
	renderer->Initialize();

	//コライダーマネージャー
	ColliderManager* colliderManager = ColliderManager::GetInstance();
	colliderManager->Initialize();

	//テクスチャマネージャー
	TextureManager::GetInstance()->Initialize();

	//モデルマネージャー
	ModelManager::GetInstance()->Initialize();

	//シーンを設定
	sceneManager->ChangeScene(SceneManager::kParticleEditor);

	///-------------------------------------------/// 
	/// メインループ
	///-------------------------------------------///

	while (true) {

		//ウィンドウから終了メッセージが来たらループを抜ける
		if (winApp->ProcessMessage()) {
			break;
		}

		///-------------------------------------------/// 
		/// 更新処理
		///-------------------------------------------///

		//ImGuiの受付開始
		imGuiManager->Begin();

		//入力の更新
		input->Update();

#ifdef _DEBUG

		//シーンのImGui
		sceneManager->ImGui();

		//オフスクリーンのImGui
		offScreen->ImGui();

#endif // _DEBUG

		//3dオブジェクト基底の更新
		object3DCommon->Update();

		//シーンの更新
		sceneManager->Update();

		//パーティクルの更新
		particleManager->Update();

		//コライダーマネージャーの更新
		colliderManager->Update();

		//音声の更新
		audio->Update();

		//ImGuiの受付終了
		imGuiManager->End();

		///-------------------------------------------/// 
		/// 描画処理
		///-------------------------------------------///

		/// === オフスクリーンの描画 === ///

		//OffScreenの描画前処理
		offScreen->PreDraw();

		//SRVマネージャーの描画前処理
		srvManager->PreDraw();

		//シーンの描画
		sceneManager->Draw();

		//パーティクルの描画
		particleManager->Draw();

		//OffScreen用のレンダラーの描画
		renderer->OffScreenDraw();

		//OffScreenの描画後処理
		offScreen->PostDraw();

		/// === SwapChainの描画 === ///

		//DirectX基底の描画前処理
		directXCommon->PreDraw();

		//SRVマネージャーの描画前処理
		srvManager->PreDraw();

		//オフスクリーンの描画結果をSwapChainに転送
		offScreen->DrawToSwapChain();

		//レンダラーの描画
		renderer->Draw();

#ifdef _DEBUG

		//ImGuiの描画
		imGuiManager->Draw();

#endif // _DEBUG

		//DirectX基底の描画後処理
		directXCommon->PostDraw();

	}

	///-------------------------------------------/// 
	/// 終了処理
	///-------------------------------------------///

	//音声の終了処理
	audio->Finalize();

	//ImGuiの終了処理
	imGuiManager->Finalize();

	//ウィンドウの終了処理
	winApp->Finalize();

	return 0;
}
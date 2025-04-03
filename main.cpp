#include "Base/WinApp.h"
#include "Base/DirectXCommon.h"
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

	//ImGuiマネージャー
	ImGuiManager* imGuiManager = ImGuiManager::GetInstance();
	imGuiManager->Initialize();

	//SRVマネージャー
	SrvManager* srvManager = SrvManager::GetInstance();
	srvManager->Initialize();

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

	//モデル基底
	ModelCommon* modelCommon = ModelCommon::GetInstance();
	modelCommon->Initialize();

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

	//テクスチャマネージャー
	TextureManager::GetInstance()->Initialize();

	//モデルマネージャー
	ModelManager::GetInstance()->Initialize();

	//シーンを設定
	sceneManager->ChangeScene(SceneManager::kGame);

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

		//3dオブジェクト基底の更新
		object3DCommon->Update();

		//シーンの更新
		sceneManager->Update();

		//パーティクルの更新
		particleManager->Update();

		//音声の更新
		audio->Update();

		//ImGuiの受付終了
		imGuiManager->End();

		///-------------------------------------------/// 
		/// 描画処理
		///-------------------------------------------///

		//DirectX基底の描画前処理
		directXCommon->PreDraw();

		//SRVマネージャーの描画前処理
		srvManager->PreDraw();

		//シーンの描画
		sceneManager->Draw();

		//レンダラーの描画
		renderer->Draw();

		//パーティクルの描画
		particleManager->Draw();

		//ImGuiの描画
		imGuiManager->Draw();

		//DirectX基底の描画後処理
		directXCommon->PostDraw();

	}

	///-------------------------------------------/// 
	/// 終了処理
	///-------------------------------------------///

	audio->Finalize();

	imGuiManager->Finalize();

	winApp->Finalize();

	return 0;
}
#include "GameScene.h"

#include "Base/OffScreen.h"
#include "Base/Input.h"
#include "Scene/SceneManager.h"
#include "ObjectManager.h"

#include "2d/Sprite/SpriteManager.h"
#include "3d/Model/ModelManager.h"
#include "2d/Sprite/TextureManager.h"

#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"
#include "3d/Object/SkyBoxCommon.h"

#include "LevelEditor/LevelDataLoader.h"
#include "Fade/Fade.h"
#include "Shake/Shake.h"
#include "Flash/Flash.h"

#include "Math/Easing.h"

#include "imgui.h"

#include "numbers"

///=====================================================/// 
/// 初期化
///=====================================================///
void GameScene::Initialize() {

	object3D_ = std::make_unique<GameScene3DObject>();

	object3D_->Initialize();

	object2D_ = std::make_unique<GameScene2DObject>();

	object2D_->Initialize();

	startEvent_ = std::make_unique<StartEvent>();

	startEvent_->Initialize(object3D_.get(), object2D_.get());

	clearEvent_ = std::make_unique<ClearEvent>();

	clearEvent_->Initialize(object3D_.get(), object2D_.get());

	gameOverEvent_ = std::make_unique<GameOverEvent>();

	gameOverEvent_->Initialize(object3D_.get(), object2D_.get());

}

///=====================================================/// 
/// 終了処理
///=====================================================///
void GameScene::Finalize() {

	ObjectManager::GetInstance()->ClearAll();

	//演出系の参照リセット
	Fade::GetInstance()->SetCamera(nullptr);

	Fade::GetInstance()->SetPlayer(nullptr);

	Shake::GetInstance()->SetCamera(nullptr);

	//音声データの解放
	Audio::GetInstance()->Finalize();
}

///=====================================================/// 
/// 更新
///=====================================================///
void GameScene::Update() {

	if (Fade::GetInstance()->GetState() == Fade::FADE_IN_END) {

		Fade::GetInstance()->SetState(Fade::NONE);
	}

	startEvent_->Update();

	clearEvent_->Update();

	gameOverEvent_->Update();

	object3D_->Update();

	object2D_->Update();

	//フェードアウトが終わったら
	if (Fade::GetInstance()->GetState() == Fade::FADE_OUT_END) {

		//フェードの状態をリセット
		Fade::GetInstance()->SetState(Fade::NONE);

		//タイトルシーンに遷移
		SceneManager::GetInstance()->ChangeScene(SceneManager::kTitle);
	}
}

///=====================================================/// 
/// 描画
///=====================================================///
void GameScene::Draw() {

	object3D_->Draw();

	object2D_->Draw();

}

///=====================================================/// 
/// ImGui
///=====================================================///
void GameScene::ImGui() {

#ifdef _USE_IMGUI

	//ImGuiを起動
	ImGui::Begin("Scene");

	if (ImGui::TreeNode("Camera")) {

		camera_->DisplayImGui();

		ImGui::TreePop();
	}

	ImGui::Text("Shift + LeftClick : Move Camera");
	ImGui::Text("Shift + RightClick : Rotate Camera");
	ImGui::Text("Shift + MiddleWheel : Move Offset Camera");

	//ImGuiの終了
	ImGui::End();

#endif // _USE_IMGUI

}
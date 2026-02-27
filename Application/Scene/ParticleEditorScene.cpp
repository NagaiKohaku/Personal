#include "ParticleEditorScene.h"

#include "2d/Sprite/SpriteManager.h"
#include "2d/Sprite/TextureManager.h"
#include "3d/Model/ModelManager.h"

#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"

#include "imgui.h"
#include "fstream"
#include "filesystem"

#include <Scene/EngineContext.h>

using namespace MyEngine;

///=====================================================/// 
/// パーティクルエディタ用シーンの初期化
///=====================================================///
void ParticleEditorScene::Initialize(EngineContext context) {

	/// === カメラの設定 === ///

	BaseScene::Initialize(context);

	context_.camera->SetDebugCameraFlag(true);

	context_.camera->SetOffsetZ(-30.0f);

	context_.camera->SetRotate({ 0.6f,-0.3f,0.0f });

	//デフォルトカメラを設定
	context_.objectCommon.object3DCommon->SetDefaultCamera(context_.camera);

	context_.objectCommon.debugObjectCommon->SetDefaultCamera(context_.camera);

	/// === テクスチャの読み込み === ///

	//テクスチャディレクトリからテクスチャファイル名を取得
	for (const auto& entry : std::filesystem::directory_iterator("Resource/Sprite/Particle/")) {

		if (entry.path().extension() == ".png") {

			textureList_.push_back(entry.path().filename().string());
		}
	}

	//エミッターグループディレクトリからグループ名を取得
	for (const auto& entry : std::filesystem::directory_iterator("Resource/Json/Particle/Group/")) {

		if (entry.is_directory()) {

			emitterGroupNames_.push_back(entry.path().filename().string());
		}
	}

	//テクスチャの読み込み
	for (auto& textureName : textureList_) {

		TextureManager::GetInstance()->LoadTexture("Resource/Sprite/Particle/" + textureName);
	}

	//デフォルトグループの生成
	CreateGroup();

	/// === 床オブジェクトの生成 === ///

	//ラインの分割数を設定
	lineDivide_ = 12.0f;

	//縦線の生成
	for (size_t i = 0; i < lineDivide_ + 1; i++) {

		std::unique_ptr<DebugLine> newLine = std::make_unique<DebugLine>();

		newLine->Initialize(
			context_.objectCommon.debugObjectCommon,
			{ i - lineDivide_ / 2.0f,0.0f,-lineDivide_ / 2.0f },
			{ i - lineDivide_ / 2.0f,0.0f,lineDivide_ / 2.0f },
			{ 1.0f,1.0f,1.0f,1.0f }
		);

		if (i == static_cast<int>(lineDivide_ / 2.0f)) {
			newLine->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		}

		lines_.push_back(std::move(newLine));
	}

	//横線の生成
	for (size_t i = 0; i < lineDivide_ + 1; i++) {

		std::unique_ptr<DebugLine> newLine = std::make_unique<DebugLine>();

		newLine->Initialize(
			context_.objectCommon.debugObjectCommon,
			{ -lineDivide_ / 2.0f,0.0f,i - lineDivide_ / 2.0f },
			{ lineDivide_ / 2.0f,0.0f,i - lineDivide_ / 2.0f },
			{ 1.0f,1.0f,1.0f,1.0f }
		);

		if (i == static_cast<int>(lineDivide_ / 2.0f)) {
			newLine->SetColor({ 0.0f,1.0f,0.0f,1.0f });
		}

		lines_.push_back(std::move(newLine));
	}
}

///=====================================================/// 
/// パーティクルエディタシーンの毎フレーム更新処理
///=====================================================///
void ParticleEditorScene::Update() {

	//カメラをデバッグ状態で更新
	context_.camera->Update();

	//エミッターグループの更新
	for (auto& group : emitterGroups_) {

		group->Update();
	}

	//デバッグラインの更新
	for (auto& line : lines_) {

		line->Update();
	}
}

///=====================================================/// 
/// パーティクルエディタシーンの描画処理
///=====================================================///
void ParticleEditorScene::Draw() {

	//エミッターグループの描画
	for (auto& group : emitterGroups_) {

		group->Draw();
	}

	//デバッグラインの描画
	for (auto& line : lines_) {

		line->Draw(LayerType::OBJECT);
	}
}

///=====================================================/// 
/// パーティクルエディタシーンのImGui表示処理
///=====================================================///
void ParticleEditorScene::ImGui() {

#ifdef _USE_IMGUI

	//ウィンドウの開始
	ImGui::Begin("ParticleEditor", nullptr, ImGuiWindowFlags_MenuBar);

	//メニューバーの作成
	if (ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("メニュー", "MENU")) {

			//グループ読み込みメニューの作成
			if (ImGui::BeginMenu("グループを読み込み", "LOAD")) {

				//登録されているグループ名をメニューに表示
				for (auto& groupName : emitterGroupNames_) {

					//グループ名が選ばれたら
					if (ImGui::MenuItem(groupName.c_str())) {

						//グループを読み込む
						LoadGroup(groupName);
					}
				}

				//グループ読み込みメニュー終了
				ImGui::EndMenu();
			}

			//新規グループ生成メニュー
			if (ImGui::MenuItem("新しいグループを生成")) {

				//新しいグループを生成
				LoadGroup("defaultGroup");
			}

			//新規グループ生成メニュー終了
			ImGui::EndMenu();
		}

		//メニューバー終了
		ImGui::EndMenuBar();
	}

	//エミッターグループのImGui表示
	for (auto& group : emitterGroups_) {

		group->ImGui();
	}

	ImGui::End();

#endif // _USE_IMGUI

}

///=====================================================/// 
/// デフォルトのパーティクルエミッターグループを生成
///=====================================================///
void ParticleEditorScene::CreateGroup() {

	//デフォルトグループの生成
	std::unique_ptr<EmitterGroup> newGroup = std::make_unique<EmitterGroup>();

	newGroup->Initialize(context_.objectCommon.particleCommon, context_.camera);

	newGroup->SetTextureList(textureList_);

	newGroup->LoadEmitter("defaultGroup");

	emitterGroups_.push_back(std::move(newGroup));
}

///=====================================================/// 
/// 指定した名前のパーティクルエミッターグループを読み込み
///=====================================================///
void ParticleEditorScene::LoadGroup(const std::string& groupName) {

	//新しいグループの生成
	std::unique_ptr<EmitterGroup> newGroup = std::make_unique<EmitterGroup>();

	//グループの初期化
	newGroup->Initialize(context_.objectCommon.particleCommon, context_.camera);

	//テクスチャリストの設定
	newGroup->SetTextureList(textureList_);

	//エミッターの読み込み
	newGroup->LoadEmitter(groupName);

	//グループの追加
	emitterGroups_.push_back(std::move(newGroup));
}
#include "GroundManager.h"

#include <Object/Ground/Ground.h>
#include <Object/Ground/Building.h>
#include "3d/Object/Object3D.h"
#include "3d/Model/Model.h"

#include <Math/Utility/Random.h>

#include "imgui.h"

using namespace MyEngine;

GroundManager* GroundManager::GetInstance() {
	static GroundManager instance;
	return &instance;
}

///=====================================================/// 
/// 地面と建物群を初期化
///=====================================================///
void GroundManager::Initialize() {

	//建物のパラメータ設定
	buildingWidth_ = 8.0f;

	buildingHeight_ = 20.0f;

	buildingRandomRange_ = 6.0f;

	buildingDistance_ = 9.0f;

	buildingStartX_ = 30.0f;

	buildingStartZ_ = 1000.0f;

	wallPos_ = { 0.0f,0.0f,1000.0f };

	maxBuildingNum_ = 200;

	//地面の生成
	ground_ = std::make_unique<Ground>();

	ground_->Initialize();

	//1列あたりの建物の数
	int buildingNumOfLine = maxBuildingNum_ / 2;

	//建物の生成
	for (int i = 0; i < buildingNumOfLine; i++) {
		for (int j = 0; j < maxBuildingNum_ / buildingNumOfLine; j++) {

			//建物の生成
			std::unique_ptr<Building> building = std::make_unique<Building>();

			//建物のスケール計算
			Vector3 scale = { buildingWidth_,buildingHeight_ + RandomFloat(-buildingRandomRange_,buildingRandomRange_),buildingWidth_ };

			//建物の座標計算
			Vector3 pos = {
				buildingStartX_ + j / 2 * (buildingWidth_ * 2.0f + buildingDistance_),
				0.0f,
				buildingStartZ_ - i * (buildingWidth_ * 2.0f + buildingDistance_)
			};

			if (j % 2 == 0) {
				pos.x *= -1.0f; // 偶数番目の建物はX座標を反転
			}

			//建物の初期化
			building->Initialize(pos, scale);

			//建物をコンテナに追加
			building_.push_back(std::move(building));
		}

		//終了判定
		if (buildingStartZ_ - i * (buildingWidth_ * 2.0f + buildingDistance_) <= -buildingStartZ_) {

			break;
		}
	}
}

///=====================================================/// 
/// 地面と建物群の後片付け
///=====================================================///
void GroundManager::Finalize() {

	//建物オブジェクトコンテナのクリア
	building_.clear();
}

///=====================================================/// 
/// 地面と建物群の更新処理
///=====================================================///
void GroundManager::Update() {

	damageGround_.remove_if([](const std::unique_ptr<DamageGround>& damageGround) {
		if (damageGround->GetIsDead()) {
			return true;
		}
		return false;
		});

	//地面の更新
	ground_->Update();

	//建物の更新
	for (auto& building : building_) {

		if (building->GetWorldTransform().translate_.z < -buildingStartZ_) {

			building->SetPosZ(buildingStartZ_);
		}

		building->Update();
	}

	for (auto& damageGround : damageGround_) {

		damageGround->Update();
	}

}

///=====================================================/// 
/// 地面と建物群の座標更新
///=====================================================///
void GroundManager::TransformUpdate() {

	//地面の座標更新
	ground_->TransformUpdate();

	//建物の座標更新
	for (auto& building : building_) {

		building->TransformUpdate();
	}

	for (auto& damageGround : damageGround_) {

		damageGround->TransformUpdate();
	}
}

///=====================================================/// 
///  地面と建物群を描画
///=====================================================///
void GroundManager::Draw() {

	//地面の描画
	ground_->Draw();

	//建物の描画
	for (auto& building : building_) {

		building->Draw();
	}

	for (auto& damageGround : damageGround_) {

		damageGround->Draw();
	}
}

///=====================================================/// 
/// ImGuiによるパラメータ調整
///=====================================================///
void GroundManager::ImGui() {

#ifdef _USE_IMGUI

#endif // _USE_IMGUI

}

void GroundManager::CreateDamageGround(MyEngine::Vector3 pos) {

	std::unique_ptr<DamageGround> newDamageGround;

	newDamageGround = std::make_unique<DamageGround>();

	newDamageGround->Initialize(pos);

	damageGround_.push_back(std::move(newDamageGround));
}
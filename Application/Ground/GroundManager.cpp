#include "GroundManager.h"

#include "Ground/Ground.h"
#include "Ground/Building.h"
#include "3d/Object/Object3D.h"
#include "3d/Model/Model.h"

#include "Math/Random.h"

#include "imgui.h"

void GroundManager::Initialize() {

	radius_ = 8.0f;

	height_ = 20.0f;

	distance_ = 9.0f;

	startPosX_ = 30.0f;

	startPosZ_ = 1000.0f;

	//地面の生成
	ground_ = std::make_unique<Ground>();

	ground_->Initialize();

	//建物の生成
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 2; j++) {

			//建物の生成
			std::unique_ptr<Building> building = std::make_unique<Building>();

			Vector3 scale = { radius_,height_ + RandomFloat(-6.0f,6.0f),radius_};

			//建物の座標計算
			Vector3 pos = {
				startPosX_ + j / 2 * (radius_ * 2.0f + distance_),
				0.0f,
				startPosZ_ - i * (radius_ * 2.0f + distance_)
			};

			if (j % 2 == 0) {
				pos.x *= -1.0f; // 偶数番目の建物はX座標を反転
			}

			//建物の初期化
			building->Initialize(pos, scale);

			//建物をコンテナに追加
			building_.push_back(std::move(building));
		}

		if (startPosZ_ - i * (radius_ * 2.0f + distance_) <= -startPosZ_) {

			break;
		}
	}
}

void GroundManager::Finalize() {
	building_.clear();
}

void GroundManager::Update() {

	ground_->Update();

	for (auto& building : building_) {

		if (building->GetWorldTransform().translate_.z < -startPosZ_) {

			building->SetPosZ(startPosZ_);
		}

		building->Update();
	}
}

void GroundManager::TransformUpdate() {

	ground_->TransformUpdate();

	for (auto& building : building_) {

		building->TransformUpdate();
	}
}

void GroundManager::Draw() {

	ground_->Draw();

	for (auto& building : building_) {
		building->Draw();
	}
}

void GroundManager::ImGui() {

#ifdef _USE_IMGUI

	ImGui::Begin("GroundManager");

	for (auto& building : building_) {
		ImGui::Text("Pos : (%f, %f, %f)",
			building->GetWorldTransform().translate_.x,
			building->GetWorldTransform().translate_.y,
			building->GetWorldTransform().translate_.z
		);
	}

	ImGui::End();

#endif // _USE_IMGUI

}
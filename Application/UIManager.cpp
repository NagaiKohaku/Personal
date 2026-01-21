#include "UIManager.h"

#include <2d/Sprite/SpriteManager.h>

#include <Base/Input.h>

#include <imgui.h>

#include <json.hpp>
#include <filesystem>
#include <fstream>

UIManager* UIManager::GetInstance() {
	static UIManager instance;
	return &instance;
}

void UIManager::Initialize() {

	directoryPath_ = "Resource/Json/UIGroup/";

	resizeMargin_ = 10.0f;

	isEditMode_ = false;
}

void UIManager::Update() {

	if (isEditMode_) {

		Edit();
	}

	for (auto& uiGroup : uiGroups_) {
		for (auto& ui : uiGroup->uiList) {
			ui.object->Update();
		}
	}

}

void UIManager::Draw() {

	for (auto& uiGroup : uiGroups_) {
		for (auto& ui : uiGroup->uiList) {
			ui.object->Draw(LayerType::UI);
		}
	}
}

void UIManager::ImGui() {

#ifdef _USE_IMGUI

	//ウィンドウの開始
	ImGui::Begin("UIManager", nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("メニュー", "MENU")) {

			if (ImGui::MenuItem("保存")) {

				SaveUIState("TitleScene");
			}

			if (ImGui::MenuItem("追加")) {

			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::Checkbox("編集を有効化", &isEditMode_)) {
		EnableEditMode(isEditMode_);
	}

	ImGui::Separator();

	for (auto& uiGroup : uiGroups_) {

		std::string groupLabelStr = uiGroup->name + " : UI数[" + std::to_string(uiGroup->uiList.size()) + "]";
		const char* groupLabel = groupLabelStr.c_str();

		if (ImGui::TreeNode(groupLabel)) {

			for (auto& ui : uiGroup->uiList) {

				if (ImGui::TreeNode(ui.name.c_str())) {

					Vector2 position = ui.object->GetTranslate();

					ImGui::Text("座標");
					ImGui::DragFloat2("##Position", &position.x, 0.1f);
					ImGui::NextColumn();

					ui.object->SetTranslate(position);

					float rotate = ui.object->GetRotate();

					ImGui::Text("回転");
					ImGui::DragFloat("##Rotate", &rotate, 0.1f);
					ImGui::NextColumn();

					ui.object->SetRotate(rotate);

					Vector2 size = ui.object->GetSize();

					ImGui::Text("サイズ");
					ImGui::DragFloat2("##Size", &size.x, 0.1f);
					ImGui::NextColumn();

					ui.object->SetSize(size);

					ImGui::TreePop();

				}
			}

			ImGui::TreePop();
		}
	}

	//ImGuiの終了
	ImGui::End();

#endif // _USE_IMGUI

}

void UIManager::CreateUI(const std::string& groupName, const std::string uiName, const std::string& spriteName) {

	UIObject newUIObject;

	newUIObject.name = uiName;

	newUIObject.object = std::make_unique<Object2D>();

	newUIObject.object->Initialize();

	newUIObject.object->SetSprite(spriteName);

	CreateUIGroup(groupName);

	GetUIGroup(groupName)->uiList.push_back(std::move(newUIObject));
}

void UIManager::DeleteUI(const std::string& groupName) {

	UIGroup* group = GetUIGroup(groupName);

	for (auto it = uiGroups_.begin(); it != uiGroups_.end(); it++) {

		if (it->get()->name == groupName) {

			for (auto& ui : it->get()->uiList) {

				ui.object.reset();
			}

			it = uiGroups_.erase(it);

			return;
		}
	}

}

void UIManager::Edit() {

	for (auto& uiGroup : uiGroups_) {
		for (auto& ui : uiGroup->uiList) {

			Vector2 uiMin = { ui.object->GetTranslate().x - ui.object->GetSize().x / 2.0f, ui.object->GetTranslate().y - ui.object->GetSize().y / 2.0f };
			Vector2 uiMax = { ui.object->GetTranslate().x + ui.object->GetSize().x / 2.0f, ui.object->GetTranslate().y + ui.object->GetSize().y / 2.0f };

			Vector2 mousePos = Input::GetInstance()->GetMousePos();

			EditUIPosition(ui.object.get(), uiMin, uiMax, mousePos);

			EditUISize(ui.object.get(), uiMin, uiMax, mousePos);
		}
	}
}

void UIManager::EditUIPosition(Object2D* uiObject, Vector2 uiMin, Vector2 uiMax, Vector2 mousePos) {

	// 左クリックの押下状態を取得
	bool isLeftDown = Input::GetInstance()->IsPushMouseButton(0);

	if (!isLeftDown) {

		// ドラッグ終了
		activeRepositioningUI_ = nullptr;

		return;
	}

	// ドラッグを開始する条件：現在無効で、マウスが範囲内かつ左ボタンが押されている
	if (activeRepositioningUI_ == nullptr) {

		// マウスがUI内にあるかどうか（開始判定にのみ使用）
		bool inHitZone = CheckInZone(uiMin, uiMax, mousePos);

		if (inHitZone && isLeftDown) {

			// ドラッグを開始
			activeRepositioningUI_ = uiObject;
		}
	}

	// ここからは activeRepositioningUI_ == uiObject のときに処理される
	if (activeRepositioningUI_ == uiObject) {

		// UIの座標をマウス座標に合わせる（元実装と同様に中央を合わせる）
		uiObject->SetTranslate({ mousePos.x, mousePos.y });
	}
}

void UIManager::EditUISize(Object2D* uiObject, Vector2 uiMin, Vector2 uiMax, Vector2 mousePos) {

	// 右クリックの押下状態（押されている間 true とする既存仕様を仮定）
	bool isRightDown = Input::GetInstance()->IsPushMouseButton(1);

	if (!isRightDown) {

		// ドラッグ終了
		activeResizingUI_ = nullptr;

		return;
	}

	// マウスがリサイズ判定領域内かどうか（開始判定にのみ使う）
	bool inResizeZone = CheckInZone(uiMin, uiMax, mousePos);

	if (activeResizingUI_ == nullptr) {

		if (inResizeZone && isRightDown) {

			// ドラッグ開始
			activeResizingUI_ = uiObject;
		}
	}

	// リサイズ中の処理（対象が一致する場合のみ）
	if (activeResizingUI_ == uiObject) {

		Vector2 texSize = activeResizingUI_->GetSprite()->GetTextureSize();

		// マウス位置とUI中心の相対距離を、初期サイズの半分で正規化
		Vector2 sizeRatio = { std::abs(mousePos.x - activeResizingUI_->GetTranslate().x) / (texSize.x / 2.0f),
						std::abs(mousePos.y - activeResizingUI_->GetTranslate().y) / (texSize.y / 2.0f) };

		// スケール係数（初期距離に対する比）
		float scaleRatio = (sizeRatio.x + sizeRatio.y) * 0.5f;

		// 新しいサイズを計算（アスペクト比を変えない：両軸に同じ係数を適用）
		Vector2 newSize = { texSize.x * scaleRatio, texSize.y * scaleRatio };

		// サイズの下限を確保（幅・高さともに小さすぎないように）
		const float minSize = 1.0f;
		if (newSize.x < minSize) {
			newSize.x = minSize;
		}
		if (newSize.y < minSize) {
			newSize.y = minSize;
		}

		// サイズを設定（translate は変更しない）
		activeResizingUI_->SetSize(newSize);
	}
}

bool UIManager::CheckInZone(Vector2 min, Vector2 max, Vector2 mousePos) {

	if (mousePos.x >= min.x && mousePos.x <= max.x &&
		mousePos.y >= min.y && mousePos.y <= max.y) {
		return true;
	}

	return false;
}

void UIManager::EnableEditMode(bool flag) {

	for (auto& uiGroup : uiGroups_) {
		for (auto& ui : uiGroup->uiList) {

			ui.object->GetSprite()->SetEnableEdit(flag);
		}
	}
}

void UIManager::CreateUIGroup(const std::string& groupName) {

	if (GetUIGroup(groupName) != nullptr) {
		return;
	}

	std::unique_ptr<UIGroup> newGroup = std::make_unique<UIGroup>();

	newGroup->name = groupName;

	uiGroups_.push_back(std::move(newGroup));
}

void UIManager::LoadUI(const std::string& fileName) {

	nlohmann::json jsonData;

	std::string directoryPath = directoryPath_ + fileName + "/";

	std::string filePath = directoryPath + fileName + ".json";

	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::string message = "Failed open data file for read.";
		MessageBoxA(nullptr, message.c_str(), "EmitterGroup", 0);
		assert(0);
		return;
	}

	file >> jsonData;

	file.close();

	for (auto& [groupName, groupData] : jsonData.items()) {

		CreateUIGroup(groupName);

		for (auto& [uiName, uiData] : groupData.items()) {

			std::string uiFileName;

			uiFileName = uiData["fileName"].get<std::string>();

			CreateUI(groupName, uiName, uiFileName);

			Object2D* ui = GetUIObject(groupName, uiName);

			Vector2 position{
				uiData["position"][0].get<float>(),
				uiData["position"][1].get<float>()
			};

			float rotate = uiData["rotate"].get<float>();

			Vector2 size{
				uiData["size"][0].get<float>(),
				uiData["size"][1].get<float>()
			};

			Vector4 color{
				uiData["color"][0].get<float>(),
				uiData["color"][1].get<float>(),
				uiData["color"][2].get<float>(),
				uiData["color"][3].get<float>()
			};

			ui->SetTranslate(position);
			ui->SetRotate(rotate);
			ui->SetSize(size);
			ui->GetSprite()->SetColor(color);
		}
	}
}

void UIManager::SaveUIState(const std::string& fileName) {

	nlohmann::json jsonData;

	std::string directoryPath = directoryPath_ + fileName + "/";

	std::string filePath = directoryPath + fileName + ".json";

	for (auto& uiGroup : uiGroups_) {
		for (auto& ui : uiGroup->uiList) {

			std::string groupName = uiGroup->name;
			std::string uiName = ui.name;

			Vector2 uiPosition = ui.object->GetTranslate();
			float uiRotate = ui.object->GetRotate();
			Vector2 uiSize = ui.object->GetSize();
			Vector4 uiColor = ui.object->GetSprite()->GetColor();
			std::string uiFileName = ui.object->GetSprite()->GetFileName();

			jsonData[groupName][uiName] = {
				{ "position", {uiPosition.x,uiPosition.y}},
				{ "rotate",    uiRotate},
				{ "size",     {uiSize.x, uiSize.y}},
				{ "color",    {uiColor.x, uiColor.y, uiColor.z, uiColor.w}},
				{ "fileName",  uiFileName}
			};
		}
	}

	std::filesystem::path dir(directoryPath);

	if (!std::filesystem::exists(directoryPath)) {

		std::filesystem::create_directory(directoryPath);
	}

	std::ofstream file;

	file.open(filePath);

	if (file.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "UIManager", 0);
		assert(0);
		return;
	}

	file << jsonData.dump(4);

	file.close();
}

UIManager::UIGroup* UIManager::GetUIGroup(const std::string groupName) {

	for (auto& uiGroup : uiGroups_) {
		if (uiGroup->name.compare(groupName) == 0) {
			return uiGroup.get();
		}
	}

	return nullptr;
}

Object2D* UIManager::GetUIObject(const std::string groupName, const std::string uiName) {

	UIGroup* uiGroup = GetUIGroup(groupName);

	for (auto& ui : uiGroup->uiList) {
		if (ui.name.compare(uiName) == 0) {
			return ui.object.get();
		}
	}

	return nullptr;
}

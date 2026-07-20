#include <UI/UIManager.h>

#include <2d/Sprite/SpriteManager.h>

#include <Base/Input.h>

#include <imgui.h>

#include <json.hpp>
#include <filesystem>
#include <fstream>

using namespace MyEngine;

UIManager* UIManager::GetInstance() {
	static UIManager instance;
	return &instance;
}

void UIManager::Initialize(Camera* cameraPtr) {

	camera_ = cameraPtr;

	spriteNameList_ = SpriteManager::GetInstance()->GetSpriteNameList();

	directoryPath_ = "Resource/Json/UIGroup/";

	initialPos_ = { 640.0f, 360.0f };

	stringBufSize_ = static_cast<size_t>(256);

	resizeMargin_ = 10.0f;

	isEditMode_ = false;

	isSave_ = false;
}

void UIManager::Update() {

	Edit();

	for (auto& uiGroup : uiGroups_) {

		uiGroup->transform.UpdateMatrix();

		for (auto& ui : uiGroup->uiList) {

			ui.object->Update();
		}
	}

}

void UIManager::Draw() {

	for (auto& uiGroup : uiGroups_) {

		if (!uiGroup->isActive) {
			continue;
		}

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

				isSave_ = true;
			}

			if (ImGui::MenuItem("UIグループ追加")) {

				CreateUIGroup("NewGroup");
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Separator();

	for (auto& uiGroup : uiGroups_) {

		std::string groupLabelStr = uiGroup->name + " : UI数[" + std::to_string(uiGroup->uiList.size()) + "]";
		const char* groupLabel = groupLabelStr.c_str();

		if (ImGui::TreeNode(groupLabel)) {

			std::string inputUIGroupName = uiGroup->name;

			ImGui::Text("UIグループ名");
			if (ImGui::InputText("##UIGroupName", inputUIGroupName.data(), stringBufSize_)) {

				if (Input::GetInstance()->IsTriggerPushKey(DIK_RETURN)) {

					uiGroup->name = inputUIGroupName.c_str();
				}
			}
			ImGui::NextColumn();
			ImGui::Separator();

			ImGui::Text("グループの有効化");
			ImGui::Checkbox("##GroupActive", &uiGroup->isActive);
			ImGui::NextColumn();
			ImGui::Separator();

			ImGui::Text("グループ座標");
			ImGui::DragFloat2("##GroupPosition", &uiGroup->transform.translate_.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("グループ回転");
			ImGui::DragFloat2("##GroupRotate", &uiGroup->transform.rotate_.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("グループサイズ");
			ImGui::DragFloat2("##GroupSize", &uiGroup->transform.scale_.x, 0.1f);
			ImGui::NextColumn();

			if (ImGui::TreeNode("新規スプライト")) {

				ImGui::BeginChild("Sprite", ImVec2(0, 300), true);

				for (size_t i = 0; i < spriteNameList_.size(); ++i) {

					auto& it = spriteNameList_[i];

					if (ImGui::Selectable(it.c_str())) {

						CreateUI(uiGroup->name, "NewUI", it);
					}
				}

				ImGui::EndChild();

				ImGui::TreePop();
			}
			ImGui::NextColumn();
			ImGui::Separator();
			ImGui::Separator();

			for (auto& ui : uiGroup->uiList) {

				std::string inputUIName = ui.name;

				if (ImGui::TreeNode(ui.name.c_str())) {

					ImGui::Text("編集を有効化");
					if (ImGui::Checkbox("##IsEdit", &ui.isEdit)) {
						EnableEditMode(&ui, ui.isEdit);
					}
					ImGui::NextColumn();
					ImGui::Separator();

					ImGui::Text("名前");
					if (ImGui::InputText("##UIName", inputUIName.data(), stringBufSize_)) {

						if (Input::GetInstance()->IsTriggerPushKey(DIK_RETURN)) {

							ui.name = inputUIName.c_str();
						}
					}
					ImGui::NextColumn();
					ImGui::Separator();

					ImGui::Text("座標");
					ImGui::DragFloat2("##Position", &ui.uiPosition.x, 0.1f);
					ImGui::NextColumn();
					ImGui::Separator();

					ui.object->SetTranslate(ui.uiPosition);

					ImGui::Text("回転");
					ImGui::DragFloat("##Rotate", &ui.uiRotate, 0.1f);
					ImGui::NextColumn();
					ImGui::Separator();

					ui.object->SetRotate(ui.uiRotate);

					ImGui::Text("サイズ");
					ImGui::DragFloat2("##Size", &ui.uiSize.x, 0.1f);
					ImGui::NextColumn();
					ImGui::Separator();

					ui.object->SetSize(ui.uiSize);

					ImGui::Text("カラー");
					ImGui::ColorEdit4("##Color", &ui.uiColor.x);
					ImGui::NextColumn();
					ImGui::Separator();

					ui.object->GetSprite()->SetColor(ui.uiColor);

					ImGui::TreePop();

				}
			}

			ImGui::TreePop();
		}
	}

	if (isSave_) {
		ImGui::OpenPopup("Save");
	}

	if (ImGui::BeginPopupModal("Save", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

		std::string inputFileName;
		std::string saveFileName;

		ImGui::Text("保存するファイル名を入力してください。");

		if (ImGui::InputText("##InputFileName", inputFileName.data(), stringBufSize_)) {

			if (Input::GetInstance()->IsTriggerPushKey(DIK_RETURN)) {

				saveFileName = inputFileName;

				SaveUIState(inputFileName.c_str());

				isSave_ = false;

				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}

	//ImGuiの終了
	ImGui::End();

#endif // _USE_IMGUI

}

void UIManager::CreateUI(const std::string& groupName, const std::string uiName, const std::string& spriteName) {

	UIObject newUIObject;

	CreateUIGroup(groupName);

	newUIObject.name = uiName;

	newUIObject.object = std::make_unique<Object2D>();

	newUIObject.object->Initialize(camera_);

	newUIObject.object->SetSprite(spriteName);

	newUIObject.object->GetWorldTransform().SetParent(&GetUIGroup(groupName)->transform);

	newUIObject.object->SetTranslate(initialPos_);

	newUIObject.uiPosition = Vector2(0.0f, 0.0f);

	newUIObject.uiRotate = 0.0f;

	newUIObject.uiSize = newUIObject.object->GetSize();

	newUIObject.uiColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	newUIObject.isEdit = false;

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

void UIManager::DeleteAllUI() {

	while (!uiGroups_.empty()) {

		DeleteUI(uiGroups_.begin()->get()->name);
	}
}

void UIManager::Edit() {

	for (auto& uiGroup : uiGroups_) {
		for (auto& ui : uiGroup->uiList) {

			if (ui.isEdit) {

				Vector2 uiMin = { ui.object->GetTranslate().x - ui.object->GetSize().x / 2.0f, ui.object->GetTranslate().y - ui.object->GetSize().y / 2.0f };
				Vector2 uiMax = { ui.object->GetTranslate().x + ui.object->GetSize().x / 2.0f, ui.object->GetTranslate().y + ui.object->GetSize().y / 2.0f };

				Vector2 mousePos = Input::GetInstance()->GetMousePos();

				EditUIPosition(&ui, uiMin, uiMax, mousePos);

				EditUISize(&ui, uiMin, uiMax, mousePos);
			}
		}
	}
}

void UIManager::EditUIPosition(UIObject* uiObject, Vector2 uiMin, Vector2 uiMax, Vector2 mousePos) {

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
		activeRepositioningUI_->uiPosition = mousePos;
	}
}

void UIManager::EditUISize(UIObject* uiObject, Vector2 uiMin, Vector2 uiMax, Vector2 mousePos) {

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

		Vector2 texSize = activeResizingUI_->object->GetSprite()->GetTextureSize();

		// マウス位置とUI中心の相対距離を、初期サイズの半分で正規化
		Vector2 sizeRatio = { std::abs(mousePos.x - activeResizingUI_->object->GetTranslate().x) / (texSize.x / 2.0f),
						std::abs(mousePos.y - activeResizingUI_->object->GetTranslate().y) / (texSize.y / 2.0f) };

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
		activeResizingUI_->uiSize = newSize;
	}
}

bool UIManager::CheckInZone(Vector2 min, Vector2 max, Vector2 mousePos) {

	if (mousePos.x >= min.x && mousePos.x <= max.x &&
		mousePos.y >= min.y && mousePos.y <= max.y) {
		return true;
	}

	return false;
}

void UIManager::EnableEditMode(UIObject* uiObject, bool flag) {

	uiObject->object->GetSprite()->SetEnableEdit(flag);
}

void UIManager::CreateUIGroup(const std::string& groupName) {

	if (GetUIGroup(groupName) != nullptr) {
		return;
	}

	std::unique_ptr<UIGroup> newGroup = std::make_unique<UIGroup>();

	newGroup->name = groupName;

	newGroup->transform.Initialize();

	newGroup->isActive = true;

	uiGroups_.push_back(std::move(newGroup));
}

void UIManager::CreateNewUIGroup() {

	CreateUIGroup("NewGroup");
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

			if (uiName == "GroupState") {

				UIGroup* uiGroup = GetUIGroup(groupName);

				Vector3 groupPosition{
					uiData["groupPosition"][0].get<float>(),
					uiData["groupPosition"][1].get<float>(),
					uiData["groupPosition"][2].get<float>()
				};

				Vector3 groupRotate{
					uiData["groupRotate"][0].get<float>(),
					uiData["groupRotate"][1].get<float>(),
					uiData["groupRotate"][2].get<float>()
				};

				Vector3 groupSize{
					uiData["groupSize"][0].get<float>(),
					uiData["groupSize"][1].get<float>(),
					uiData["groupSize"][2].get<float>()
				};

				bool groupIsActive = uiData["groupIsActive"].get<bool>();

				uiGroup->transform.translate_ = groupPosition;
				uiGroup->transform.rotate_ = groupRotate;
				uiGroup->transform.scale_ = groupSize;
				uiGroup->isActive = groupIsActive;

				continue;
			}

			uiFileName = uiData["fileName"].get<std::string>();

			CreateUI(groupName, uiName, uiFileName);

			UIObject* ui = GetUIObject(groupName, uiName);

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

			ui->uiPosition = position;
			ui->uiRotate = rotate;
			ui->uiSize = size;
			ui->uiColor = color;

			ui->object->SetTranslate(position);
			ui->object->SetRotate(rotate);
			ui->object->SetSize(size);
			ui->object->GetSprite()->SetColor(color);
		}
	}
}

void UIManager::SaveUIState(const std::string& fileName) {

	nlohmann::json jsonData;

	std::string directoryPath = directoryPath_ + fileName + "/";

	std::string filePath = directoryPath + fileName + ".json";

	for (auto& uiGroup : uiGroups_) {

		std::string groupName = uiGroup->name;
		Vector3 groupPosition = uiGroup->transform.translate_;
		Vector3 groupRotate = uiGroup->transform.rotate_;
		Vector3 groupSize = uiGroup->transform.scale_;
		bool groupIsActive = uiGroup->isActive;

		jsonData[groupName]["GroupState"] = {
			{ "groupPosition", {groupPosition.x,groupPosition.y,groupPosition.z}},
			{ "groupRotate", {groupRotate.x,groupRotate.y,groupRotate.z}},
			{ "groupSize", {groupSize.x,groupSize.y,groupSize.z}},
			{ "groupIsActive", groupIsActive}
		};

		for (auto& ui : uiGroup->uiList) {

			std::string uiName = ui.name;
			Vector2 uiPosition = ui.uiPosition;
			float uiRotate = ui.uiRotate;
			Vector2 uiSize = ui.uiSize;
			Vector4 uiColor = ui.uiColor;
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

Object2D* UIManager::Get2DObject(const std::string groupName, const std::string uiName) {

	UIGroup* uiGroup = GetUIGroup(groupName);

	for (auto& ui : uiGroup->uiList) {
		if (ui.name.compare(uiName) == 0) {
			return ui.object.get();
		}
	}

	return nullptr;
}

UIManager::UIObject* UIManager::GetUIObject(const std::string groupName, const std::string uiName) {

	UIGroup* uiGroup = GetUIGroup(groupName);

	for (auto& ui : uiGroup->uiList) {
		if (ui.name.compare(uiName) == 0) {
			return &ui;
		}
	}

	return nullptr;
}
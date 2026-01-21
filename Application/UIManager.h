#pragma once

#include <2d/Object/Object2D.h>
#include <Math/WorldTransform.h>

#include <vector>
#include <string>
#include <memory>
#include <map>

class UIManager {

public:

	struct UIObject {
		std::string name;
		std::unique_ptr<Object2D> object;
	};

	struct UIGroup {
		std::string name;
		std::vector<UIObject> uiList;
		WorldTransform transform;
	};

public:

	static UIManager* GetInstance();

	void Initialize();

	void Update();

	void Draw();

	void ImGui();

	void CreateUI(const std::string& groupName, const std::string uiName, const std::string& spriteName);

	void LoadUI(const std::string& fileName);

	void DeleteUI(const std::string& groupName);

private:

	void Edit();

	void EditUIPosition(Object2D* uiObject, Vector2 uiMin, Vector2 uiMax, Vector2 mousePos);

	void EditUISize(Object2D* uiObject, Vector2 uiMin, Vector2 uiMax, Vector2 mousePos);

	bool CheckInZone(Vector2 uiMin, Vector2 uiMax, Vector2 mousePos);

	void EnableEditMode(bool flag);

	void CreateUIGroup(const std::string& groupName);

	void SaveUIState(const std::string& fileName);

private:

	std::string directoryPath_;

	std::vector<std::unique_ptr<UIGroup>> uiGroups_;

	//座標編集中のUIオブジェクト
	Object2D* activeRepositioningUI_ = nullptr;

	Object2D* activeResizingUI_ = nullptr;

	float resizeMargin_;

	bool isEditMode_;

public:

	UIGroup* GetUIGroup(const std::string groupName);

	Object2D* GetUIObject(const std::string groupName, const std::string uiName);

};
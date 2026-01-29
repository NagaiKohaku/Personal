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
		Vector2 uiPosition;
		float uiRotate;
		Vector2 uiSize;
		Vector4 uiColor;
		bool isEdit;
	};

	struct UIGroup {
		std::string name;
		std::vector<UIObject> uiList;
		WorldTransform transform;
		bool isActive;
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

	void DeleteAllUI();

private:

	void Edit();

	void EditUIPosition(UIObject* uiObject, Vector2 uiMin, Vector2 uiMax, Vector2 mousePos);

	void EditUISize(UIObject* uiObject, Vector2 uiMin, Vector2 uiMax, Vector2 mousePos);

	bool CheckInZone(Vector2 uiMin, Vector2 uiMax, Vector2 mousePos);

	void EnableEditMode(UIObject* uiObject, bool flag);

	void CreateUIGroup(const std::string& groupName);

	void CreateNewUIGroup();

	void SaveUIState(const std::string& fileName);

private:

	std::string directoryPath_;

	std::vector<std::unique_ptr<UIGroup>> uiGroups_;

	std::vector<std::string> spriteNameList_;

	//座標編集中のUIオブジェクト
	UIObject* activeRepositioningUI_ = nullptr;

	UIObject* activeResizingUI_ = nullptr;

	Vector2 initialPos_;

	size_t stringBufSize_;

	float resizeMargin_;

	bool isEditMode_;

	bool isSave_;

public:

	UIGroup* GetUIGroup(const std::string groupName);

	Object2D* Get2DObject(const std::string groupName, const std::string uiName);

	UIObject* GetUIObject(const std::string groupName, const std::string uiName);

};
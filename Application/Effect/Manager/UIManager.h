#pragma once

#include <2d/Object/Object2D.h>
#include <3d/Camera/Camera.h>
#include <Base/Input.h>
#include <Math/Transform/WorldTransform.h>

#include <vector>
#include <string>
#include <memory>
#include <map>

class UIManager {

public:

	struct UIObject {
		std::string name;
		std::unique_ptr<MyEngine::Object2D> object;
		MyEngine::Vector2 uiPosition;
		float uiRotate;
		MyEngine::Vector2 uiSize;
		MyEngine::Vector4 uiColor;
		bool isEdit;
	};

	struct UIGroup {
		std::string name;
		std::vector<UIObject> uiList;
		MyEngine::WorldTransform transform;
		bool isActive;
	};

public:

	static UIManager* GetInstance();

	void Initialize(MyEngine::Object2DCommon* object2DCommonPtr, MyEngine::Camera* cameraPtr, MyEngine::Input* inputPtr);

	void Update();

	void Draw();

	void ImGui();

	void CreateUI(const std::string& groupName, const std::string uiName, const std::string& spriteName);

	void LoadUI(const std::string& fileName);

	void DeleteUI(const std::string& groupName);

	void DeleteAllUI();

private:

	void Edit();

	void EditUIPosition(UIObject* uiObject, MyEngine::Vector2 uiMin, MyEngine::Vector2 uiMax, MyEngine::Vector2 mousePos);

	void EditUISize(UIObject* uiObject, MyEngine::Vector2 uiMin, MyEngine::Vector2 uiMax, MyEngine::Vector2 mousePos);

	bool CheckInZone(MyEngine::Vector2 uiMin, MyEngine::Vector2 uiMax, MyEngine::Vector2 mousePos);

	void EnableEditMode(UIObject* uiObject, bool flag);

	void CreateUIGroup(const std::string& groupName);

	void CreateNewUIGroup();

	void SaveUIState(const std::string& fileName);

private:

	MyEngine::Object2DCommon* object2DCommon_;

	//カメラ
	MyEngine::Camera* camera_;

	MyEngine::Input* input_;

	std::string directoryPath_;

	std::vector<std::unique_ptr<UIGroup>> uiGroups_;

	std::vector<std::string> spriteNameList_;

	//座標編集中のUIオブジェクト
	UIObject* activeRepositioningUI_ = nullptr;

	UIObject* activeResizingUI_ = nullptr;

	MyEngine::Vector2 initialPos_;

	size_t stringBufSize_;

	float resizeMargin_;

	bool isEditMode_;

	bool isSave_;

public:

	UIGroup* GetUIGroup(const std::string groupName);

	MyEngine::Object2D* Get2DObject(const std::string groupName, const std::string uiName);

	UIObject* GetUIObject(const std::string groupName, const std::string uiName);

};
#pragma once
#include "Scene/BaseScene.h"

#include "Base/Audio.h"
#include "2d/Object/Object2D.h"
#include "3d/Camera/Camera.h"
#include "3d/Object/Object3D.h"
#include "3d/Object/DebugLine.h"
#include "3d/Particle/ParticleEmitter.h"
#include "3d/Particle/EmitterGroup.h"
#include "3d/Object/DebugLine.h"

#include "memory"

class ParticleEditorScene : public BaseScene {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void ImGui() override;

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	void CreateGroup();

	void LoadGroup(const std::string& groupName = "defaultGroup");

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	std::unique_ptr<Camera> camera_;

	std::list<std::unique_ptr<EmitterGroup>> emitterGroups_;

	std::list<std::string> emitterGroupNames_;

	std::vector<std::unique_ptr<DebugLine>> lines_;

	std::vector<std::string> textureList_;

	float lineDivide_;

};
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

	/// <summary>
	/// パーティクルエディタ用シーンの初期化を行います。
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// パーティクルエディタシーンの毎フレーム更新処理を行います。
	/// </summary>
	void Update() override;

	/// <summary>
	/// パーティクルエディタシーンの描画処理を行います。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// パーティクルエディタシーンのImGui表示処理を行います。
	/// </summary>
	void ImGui() override;

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// デフォルトのパーティクルエミッターグループを生成します。
	/// </summary>
	void CreateGroup();

	/// <summary>
	/// 指定した名前のパーティクルエミッターグループを読み込みます。
	/// </summary>
	/// <param name="groupName">読み込むエミッターグループの名前</param>
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
#pragma once
#include "BaseScene.h"

#include "Base/Audio.h"
#include "2d/Object/Object2D.h"
#include "3d/Camera/Camera.h"
#include "3d/Object/Object3D.h"
#include "3d/Object/DebugLine.h"

#include "Player/Player.h"

#include "memory"

///=====================================================/// 
/// ゲームシーン
///=====================================================///
class GameScene : public BaseScene {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	std::unique_ptr<Camera> camera_;

	//プレイヤー
	std::unique_ptr<Player> player_;
};
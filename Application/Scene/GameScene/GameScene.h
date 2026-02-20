#pragma once
#include "Scene/BaseScene.h"

#include "Base/Audio.h"
#include "3d/Camera/Camera.h"
#include "3d/Particle/EmitterGroup.h"

#include <Object/Player/Player.h>
#include <Object/Enemy/EnemyManager.h>
#include <Object/Bullet/Core/BulletManager.h>
#include <Object/Camera/FollowCamera.h>
#include <Object/Ground/GroundManager.h>
#include "Scene/GameScene/GameSceneProgress.h"

#include "memory"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public MyEngine::BaseScene {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// ゲームシーンの初期化を行います。
	/// </summary>
	void Initialize(MyEngine::Camera* cameraPtr) override;

	/// <summary>
	/// ゲームシーンの終了処理を行います。
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ゲームシーンの毎フレーム更新処理を行います。
	/// </summary>
	void Update() override;

	/// <summary>
	/// ゲームシーンの描画処理を行います。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// デバッグ
	/// </summary>
	void ImGui() override;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	//プレイヤー
	Player* player_;

	//エネミーマネージャー
	std::unique_ptr<EnemyManager> enemyManager_;

	//バレットマネージャー
	std::unique_ptr<BulletManager> bulletManager_;

	//グラウンドマネージャー
	std::unique_ptr<GroundManager> groundManager_;

	//衝撃波エミッター(左)
	std::unique_ptr<MyEngine::EmitterGroup> shockWaveLeftEmitter_;

	//衝撃波エミッター(右)
	std::unique_ptr<MyEngine::EmitterGroup> shockWaveRightEmitter_;

	//シーンのイベント管理
	std::unique_ptr<GameSceneProgress> sceneProgress_;

	//スペースキースプライトの位置
	MyEngine::Vector2 spaceKeyPos_;

	//スペースキースプライトの大きさ
	MyEngine::Vector2 spaceKeySize_;

	//矢印の長さ
	float arrowLength_;

	//矢印のタイマー
	float arrowTimer_;

	//アニメーションタイマーの進行方向
	float timerDirection_;

};
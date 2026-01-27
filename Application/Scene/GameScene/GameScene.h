#pragma once
#include "Scene/BaseScene.h"

#include "Base/Audio.h"
#include "2d/Object/Object2D.h"
#include "3d/Camera/Camera.h"
#include "3d/Object/Object3D.h"
#include "3d/Object/DebugLine.h"
#include "3d/Object/SkyBox.h"
#include "3d/Particle/ParticleEmitter.h"
#include "3d/Particle/EmitterGroup.h"

#include "Player/Player.h"
#include "Enemy/EnemyManager.h"
#include "Bullet/BulletManager.h"
#include "Camera/FollowCamera.h"
#include "Ground/LineGround.h"
#include "Ground/Ground.h"
#include "Ground/Building.h"
#include "Ground/GroundManager.h"
#include "Scene/SceneEvent.h"

#include "memory"
#include "vector"
#include "list"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
public:

	//アニメーションのキーフレーム
	struct AnimPoint {
		Vector3 playerPos;
		Vector3 cameraRot;
		float time;
		float mag;
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// ゲームシーンの初期化を行います。
	/// </summary>
	void Initialize() override;

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

private:

	void ChangeStartEvent();

	void ChangeGameEvent();

	void ChangePauseEvent();

	void ChangeGameOverEvent();

	void ChangeClearEvent();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	std::unique_ptr<Camera> camera_;

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	//ライン
	std::vector<std::unique_ptr<DebugLine>> lines_;

	//プレイヤー
	Player* player_;

	//エネミーマネージャー
	std::unique_ptr<EnemyManager> enemyManager_;

	//バレットマネージャー
	std::unique_ptr<BulletManager> bulletManager_;

	//グラウンドマネージャー
	std::unique_ptr<GroundManager> groundManager_;

	//衝撃波エミッター(左)
	std::unique_ptr<EmitterGroup> shockWaveLeftEmitter_;

	//衝撃波エミッター(右)
	std::unique_ptr<EmitterGroup> shockWaveRightEmitter_;

	//ライン描画の地面
	std::unique_ptr<LineGround> lineGround_;

	//シーンのイベント管理
	std::unique_ptr<SceneEvent> eventState_;

	//SkyBox
	std::unique_ptr<SkyBox> skyBox_;

	//スペースキースプライトの位置
	Vector2 spaceKeyPos_;

	//スペースキースプライトの大きさ
	Vector2 spaceKeySize_;

	//矢印の長さ
	float arrowLength_;

	//矢印のタイマー
	float arrowTimer_;

	//アニメーションタイマーの進行方向
	float timerDirection_;

};
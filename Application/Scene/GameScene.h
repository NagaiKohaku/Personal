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

#include "memory"
#include "vector"
#include "list"

///=====================================================/// 
/// ゲームシーン
///=====================================================///
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

	/// <summary>
	/// デバッグ
	/// </summary>
	void ImGui() override;

	/// <summary>
	/// スタート時のアニメーション
	/// </summary>
	void StartAnimation();

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

	//ライン描画の地面
	std::unique_ptr<LineGround> lineGround_;

	//SkyBox
	std::unique_ptr<SkyBox> skyBox_;

	//ゲームオーバースプライト
	std::unique_ptr<Object2D> gameOverSprite_;

	//スペースキー
	std::unique_ptr<Object2D> spaceKeySprite_;

	//左矢印
	std::unique_ptr<Object2D> leftArrowSprite_;

	//右矢印
	std::unique_ptr<Object2D> rightArrowSprite_;

	//アニメーションのタイマー
	float timer_;

	//キーフレームの番号
	int animNum_;

	//キーフレーム
	std::vector<AnimPoint> animPoints_;

	//スタート時の演出をするかのフラグ
	bool isStart_;

	//ゲームオーバーになったかのフラグ
	bool isGameOver_;
};
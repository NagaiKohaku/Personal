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
#include "list"

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

	/// <summary>
	/// デバッグ
	/// </summary>
	void ImGui() override;

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
	std::unique_ptr<Player> player_;

	//エネミーマネージャー
	std::unique_ptr<EnemyManager> enemyManager_;

	//バレットマネージャー
	std::unique_ptr<BulletManager> bulletManager_;

	//ライン描画の地面
	std::unique_ptr<LineGround> lineGround_;

	//SkyBox
	std::unique_ptr<SkyBox> skyBox_;

	std::unique_ptr<GroundManager> groundManager_;
};
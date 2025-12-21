#pragma once

#include <AnimPoint.h>

#include <Player/Player.h>
#include <Enemy/EnemyManager.h>
#include <Bullet/BulletManager.h>
#include <Ground/GroundManager.h>
#include <3d/Particle/EmitterGroup.h>
#include <3d/Camera/Camera.h>
#include <Camera/FollowCamera.h>

#include <memory>

class GameScene3DObject {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	void Initialize();

	void Update();

	void Draw();

	void StartClearAnimation();

	void ClearAnimation(AnimPoint prePoint, AnimPoint currentPoint, float time);

	void ClearParticleEmit();

	void StartAnimation(AnimPoint prePoint, AnimPoint currentPoint, float time);

	void StartAnimEnd();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	std::unique_ptr<Camera> camera_;

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	//エネミーマネージャー
	std::unique_ptr<EnemyManager> enemyManager_;

	//バレットマネージャー
	std::unique_ptr<BulletManager> bulletManager_;

	//グラウンドマネージャー
	std::unique_ptr<GroundManager> groundManager_;

	//プレイヤー
	Player* player_;

	//衝撃波エミッター(左)
	std::unique_ptr<EmitterGroup> shockWaveLeftEmitter_;

	//衝撃波エミッター(右)
	std::unique_ptr<EmitterGroup> shockWaveRightEmitter_;

public:

	Vector3 GetPlayerPos() { return player_->GetWorldPos(); }

	bool GetPlayerIsDead() { return player_->GetIsDead(); }

};
#pragma once

#include "3d/Object/Object3D.h"
#include "3d/Particle/EmitterGroup.h"
#include "3d/Camera/Camera.h"

#include "Shadow/Shadow.h"

#include "3d/Collider/AABBCollider.h"
#include "3d/Collider/SphereCollider.h"

#include "LockOn.h"

#include "memory"

/// === 前方宣言 === ///

class LevelDataLoader;

class BulletManager;

///=====================================================/// 
/// プレイヤークラス
///=====================================================///
class Player {

	///-------------------------------------------/// 
	/// 列挙型
	///-------------------------------------------///
public:

	//移動状態
	enum MOVESTATE {
		TANK,
		JET,
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="cameraPtr">カメラポインタ</param>
	/// <param name="bulletPtr">バレットマネージャーポインタ</param>
	void Initialize(Camera* cameraPtr, BulletManager* bulletPtr);

	void Initialize(Camera* cameraPtr);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 座標のみ更新
	/// </summary>
	void TransformUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 戦車状態の移動
	/// </summary>
	void TankMove();

	/// <summary>
	/// 飛行機状態の移動
	/// </summary>
	void JetMove();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 戦車状態の攻撃
	/// </summary>
	void TankAttack();

	/// <summary>
	/// 飛行機状態の攻撃
	/// </summary>
	void JetAttack();

	/// <summary>
	/// 接触時処理
	/// </summary>
	void IsCollision();

	void Dead();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	Camera* camera_;

	//バレットマネージャー
	BulletManager* bulletManager_ = nullptr;

	//レベルデータローダー
	LevelDataLoader* levelDataLoader_;

	//オブジェクト
	std::unique_ptr<Object3D> core_ = nullptr;

	std::unique_ptr<Object3D> rightWing_;

	std::unique_ptr<Object3D> leftWing_;

	std::unique_ptr<Shadow> shadow_;

	std::vector<ObjectData> objectData_;

	std::unique_ptr<EmitterGroup> rightTrail_;

	std::unique_ptr<EmitterGroup> leftTrail_;

	std::unique_ptr<EmitterGroup> explosiveEmitter_;

	std::unique_ptr<EmitterGroup> destroyEmitter_;

	std::unique_ptr<EmitterGroup> muzzleFlashEmitter_;

	//コライダー
	std::unique_ptr<SphereCollider> collider_ = nullptr;

	//レティクル
	std::unique_ptr<LockOn> lockOn_ = nullptr;

	//移動状態
	MOVESTATE moveState_;

	//攻撃タイマー
	float attackTimer_;

	//戦車状態の攻撃間隔
	float tankAttackInterval_;

	//飛行機状態の攻撃間隔
	float jetAttackInterval_;

	//移動速度
	float moveSpeed_;

	//移動強度
	float moveStrength_;

	//回転強度
	float rotStrength_;

	//ロックオン範囲
	float lockOnRange_;

	//移動範囲
	Vector3 moveRange_;

	//戦車状態の回転範囲
	Vector3 driveRotRange_;

	//飛行機状態の回転範囲
	Vector3 flightRotRange_;

	//初期座標
	Vector3 initialPos_;

	//移動量
	Vector3 velocity_;

	bool isMoveActive;

	bool isDead_;

	bool isDestroy_;

	///-------------------------------------------///
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPos() { return core_->GetWorldTransform().GetWorldTranslate(); }

	/// <summary>
	/// 移動量を取得
	/// </summary>
	/// <returns>移動量</returns>
	Vector3 GetVelocity() { return velocity_; }

	/// <summary>
	/// 移動状態を取得
	/// </summary>
	/// <returns>移動状態</returns>
	MOVESTATE GetMoveState() { return moveState_; }

	bool GetIsDead() { return isDead_; }

	void SetPosition(Vector3 pos) { core_->GetWorldTransform().translate_ = pos; }

	void SetIsMoveActive(bool flag) { isMoveActive = flag; }
};
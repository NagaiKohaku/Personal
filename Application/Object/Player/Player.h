#pragma once

#include <Object/3D/Object3D.h>
#include "3d/Particle/EmitterGroup.h"
#include "3d/Camera/Camera.h"
#include <Base/Input.h>
#include "Scene/EngineContext.h"

#include <Object/Shadow/Shadow.h>

#include "3d/Collider/AABBCollider.h"
#include "3d/Collider/SphereCollider.h"

#include "LockOn.h"

#include <Object/Player/State/MovementState/MovementState.h>
#include <Object/Player/State/AttackState/AttackState.h>

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
	enum class MoveState {
		TANK,
		JET,
	};

	enum class EmitterType {
		RIGHTTRAIL,
		LEFTTRAIL,
		EXPLOSIVE,
		DESTROY,
		MUZZLEFLASH
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	~Player();

	/// <summary>
	/// プレイヤーを初期化します。
	/// </summary>
	void Initialize(EngineContext context, BulletManager* bulletPtr, bool isMoveActive);

	/// <summary>
	/// プレイヤーの状態を更新します。
	/// </summary>
	void Update();

	/// <summary>
	/// プレイヤーの座標のみを更新します。
	/// </summary>
	void TransformUpdate();

	/// <summary>
	/// プレイヤーの各種描画処理を行います。
	/// </summary>
	void Draw();

	void UpdateLockOn(float lockOnRange);

	void JetAttack();

	void TankAttack();

	void ChangeJetState();

	void ChangeTankState();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// プレイヤーの移動処理を行います。
	/// </summary>
	void Move();

	/// <summary>
	/// プレイヤーの攻撃処理を行います。
	/// </summary>
	void Attack();

	/// <summary>
	/// プレイヤーと他オブジェクトの衝突判定を行います。
	/// </summary>
	void IsCollision();

	/// <summary>
	/// プレイヤーが死亡した際の処理を行います。
	/// </summary>
	void Dead();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	MyEngine::Camera* camera_;

	MyEngine::Input* input_;

	//バレットマネージャー
	BulletManager* bulletManager_ = nullptr;

	//レベルデータローダー
	LevelDataLoader* levelDataLoader_;

	//オブジェクトデータ
	std::vector<ObjectData> objectData_;

	//コアオブジェクト
	std::unique_ptr<MyEngine::Object3D> core_ = nullptr;

	//右ウィングオブジェクト
	std::unique_ptr<MyEngine::Object3D> rightWing_;

	//左ウィングオブジェクト
	std::unique_ptr<MyEngine::Object3D> leftWing_;

	//エミッターリスト
	std::vector<MyEngine::EmitterGroup*> emitterList_;

	//影オブジェクト
	std::unique_ptr<Shadow> shadow_;

	//コライダー
	std::unique_ptr<MyEngine::SphereCollider> collider_ = nullptr;

	//レティクル
	std::unique_ptr<LockOn> lockOn_ = nullptr;

	//移動ステート
	std::unique_ptr<MovementState> movementState_;

	//攻撃ステート
	std::unique_ptr<AttackState> attackState_;

	//移動状態
	MoveState moveState_;

	//移動量
	MyEngine::Vector3 inputDirection_;

	//移動アクティブフラグ
	bool isMoveActive_;

	//死亡フラグ
	bool isDead_;

	//破壊フラグ
	bool isDestroy_;

	///-------------------------------------------///
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	MyEngine::Vector3 GetWorldPos() { return core_->GetWorldTransform().GetWorldTranslate(); }

	MyEngine::WorldTransform GetCoreWorldTransform() { return core_->GetWorldTransform(); }

	MyEngine::WorldTransform GetLeftWingWorldTransform() { return leftWing_->GetWorldTransform(); }

	MyEngine::WorldTransform GetRightWingWorldTransform() { return rightWing_->GetWorldTransform(); }

	MyEngine::WorldTransform GetLeftTrailWorldTransform() { return emitterList_[static_cast<size_t>(EmitterType::LEFTTRAIL)]->GetWorldTransform(); }

	MyEngine::WorldTransform GetRightTrailWorldTransform() { return emitterList_[static_cast<size_t>(EmitterType::RIGHTTRAIL)]->GetWorldTransform(); }

	/// <summary>
	/// 移動量を取得
	/// </summary>
	/// <returns>移動量</returns>
	MyEngine::Vector3 GetInputDirection() { return inputDirection_; }

	/// <summary>
	/// 移動状態を取得
	/// </summary>
	/// <returns>移動状態</returns>
	MoveState GetMoveState() { return moveState_; }

	/// <summary>
	/// 死亡フラグを取得
	/// </summary>
	/// <returns>死亡フラグ</returns>
	bool GetIsDead() { return isDead_; }

	bool GetIsMoveActive() { return isMoveActive_; }

	float GetAttackTimeRatio() {
		return attackState_->GetAttackTimer() / attackState_->GetAttackInterval();
	}

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(MyEngine::Vector3 pos) { core_->GetWorldTransform().translate_ = pos; }

	void SetCoreWorldTransform(MyEngine::WorldTransform transform) { core_->GetWorldTransform() = transform; }

	void SetLeftWingWorldTransform(MyEngine::WorldTransform transform) { leftWing_->GetWorldTransform() = transform; }

	void SetRightWingWorldTransform(MyEngine::WorldTransform transform) { rightWing_->GetWorldTransform() = transform; }

	void SetLeftTrailWorldTransform(MyEngine::WorldTransform transform) { emitterList_[static_cast<size_t>(EmitterType::LEFTTRAIL)]->GetWorldTransform() = transform; }

	void SetRightTrailWorldTransform(MyEngine::WorldTransform transform) { emitterList_[static_cast<size_t>(EmitterType::RIGHTTRAIL)]->GetWorldTransform() = transform; }

	/// <summary>
	/// 移動アクティブフラグを設定
	/// </summary>
	/// <param name="flag">移動アクティブフラグ</param>
	void SetIsMoveActive(bool flag) { isMoveActive_ = flag; }
};
#pragma once

#include "3d/Object/Object3D.h"
#include "3d/Particle/EmitterGroup.h"
#include "3d/Camera/Camera.h"

#include "Shadow/Shadow.h"

#include "3d/Collider/AABBCollider.h"
#include "3d/Collider/SphereCollider.h"

#include "LockOn.h"

#include "State/MovementState/JetMoveState.h"
#include "State/MovementState/TankMoveState.h"
#include "State/AttackState/TankAttackState.h"
#include "State/AttackState/JetAttackState.h"

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
	/// プレイヤーを初期化します。
	/// </summary>
	/// <remarks>
	/// - カメラ・弾マネージャー・レベルデータの参照を取得します。  
	/// - プレイヤーオブジェクト（本体・翼）と各エミッターを生成し、階層構造を構築します。  
	/// - コライダー、影、ロックオンシステムを初期化します。  
	/// - 攻撃間隔・移動速度・回転強度など各種パラメータを設定します。  
	/// - 初期座標を設定し、移動と生存状態を初期化します。  
	/// </remarks>
	void Initialize(Camera* cameraPtr, BulletManager* bulletPtr, bool isMoveActive);

	/// <summary>
	/// プレイヤーの状態を更新します。
	/// </summary>
	/// <remarks>
	/// - 生存状態に応じて移動形態を切り替え、移動処理を行います。  
	/// - 移動可能な場合は攻撃・衝突判定を実行します。  
	/// - 死亡時は専用処理を行います。  
	/// - 本体、ウィング、エミッター、影など関連オブジェクトを順に更新します。  
	/// - 移動可能な場合はコライダーとロックオンも更新します。  
	/// </remarks>
	void Update();

	/// <summary>
	/// プレイヤーの座標のみを更新します。
	/// </summary>
	/// <remarks>
	/// - 本体（コア）、左右ウィング、影のワールド変換を更新します。  
	/// </remarks>
	void TransformUpdate();

	/// <summary>
	/// プレイヤーの各種描画処理を行います。
	/// </summary>
	/// <remarks>
	/// - 破壊状態でなければ、コアオブジェクトや両ウィング、影の描画を行います。  
	/// - 破壊状態に関係なく、両トレイルや各種エミッターの描画を行います。  
	/// - 移動が可能な状態であれば、当たり判定用のコライダーや照準用オブジェクトの描画を行います。  
	/// </remarks>
	void Draw();

	/// <summary>
	/// マズルフラッシュエミッターを発生させます。
	/// </summary>
	void EmitMuzzleFlash() { muzzleFlashEmitter_->Emit(); }

	void ChangeJetState();

	void ChangeTankState();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// プレイヤーの移動処理を行います。
	/// </summary>
	/// <remarks>
	/// - 移動量を初期化した後、移動が可能な状態であれば入力された方向に応じて移動量を加算します。  
	/// - 移動量がある場合は、現在位置に移動量を加えた位置を求め、あらかじめ決められた範囲からはみ出さないように位置を制限します。  
	/// - 制限後の位置へ補間しながら移動し、地面より下に行かないように高さを調整します。  
	/// - 最終的な位置をオブジェクトへ反映します。  
	/// - 現在の移動状態に応じて戦車モードまたは飛行機モードの専用移動処理を呼び出します。  
	/// </remarks>
	void Move();

	/// <summary>
	/// プレイヤーの攻撃処理を行います。
	/// </summary>
	/// <remarks>
	/// - 攻撃タイマーを進め、現在の移動状態に応じて戦車モードまたは飛行機モードの攻撃処理を呼び出します。  
	/// - 各モードごとに異なる攻撃方法が適用されます。  
	/// </remarks>
	void Attack();

	/// <summary>
	/// プレイヤーと他オブジェクトの衝突判定を行います。
	/// </summary>
	/// <remarks>
	/// - コライダーが接触状態であるかを確認します。  
	/// - 接触相手が敵の弾である場合、プレイヤーの移動を無効化し、死亡フラグを立てます。  
	/// - 死亡時には爆発エミッターを発生させ、視覚的な演出を行います。  
	/// </remarks>
	void IsCollision();

	/// <summary>
	/// プレイヤーが死亡した際の処理を行います。
	/// </summary>
	/// <remarks>
	/// - プレイヤー本体を下方向に移動させつつ、コアを中心に回転させます。  
	/// - 地面に到達した場合、Y座標を固定し、破壊されていなければ破壊フラグを立てます。  
	/// - トレイルや爆発エミッターを停止し、破壊用エミッターを発生させます。  
	/// - 同時に画面揺れ演出を開始し、視覚的な死亡演出を行います。  
	/// </remarks>
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

	//オブジェクトデータ
	std::vector<ObjectData> objectData_;

	//コアオブジェクト
	std::unique_ptr<Object3D> core_ = nullptr;

	//右ウィングオブジェクト
	std::unique_ptr<Object3D> rightWing_;

	//左ウィングオブジェクト
	std::unique_ptr<Object3D> leftWing_;

	//右トレイルエミッター
	std::unique_ptr<EmitterGroup> rightTrail_;

	//左トレイルエミッター
	std::unique_ptr<EmitterGroup> leftTrail_;

	//死亡時爆発エミッター
	std::unique_ptr<EmitterGroup> explosiveEmitter_;

	//破壊時エミッター
	std::unique_ptr<EmitterGroup> destroyEmitter_;

	//マズルフラッシュエミッター
	std::unique_ptr<EmitterGroup> muzzleFlashEmitter_;

	//影オブジェクト
	std::unique_ptr<Shadow> shadow_;

	//コライダー
	std::unique_ptr<SphereCollider> collider_ = nullptr;

	//レティクル
	std::unique_ptr<LockOn> lockOn_ = nullptr;

	//移動ステート
	std::unique_ptr<MovementState> movementState_;

	//攻撃ステート
	std::unique_ptr<AttackState> attackState_;

	//移動状態
	MOVESTATE moveState_;

	//移動速度
	float moveSpeed_;

	//移動量
	Vector3 velocity_;

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
	Vector3 GetWorldPos() { return core_->GetWorldTransform().GetWorldTranslate(); }

	WorldTransform GetCoreWorldTransform() { return core_->GetWorldTransform(); }

	WorldTransform GetLeftWingWorldTransform() { return leftWing_->GetWorldTransform(); }

	WorldTransform GetRightWingWorldTransform() { return rightWing_->GetWorldTransform(); }

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

	/// <summary>
	/// 死亡フラグを取得
	/// </summary>
	/// <returns>死亡フラグ</returns>
	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(Vector3 pos) { core_->GetWorldTransform().translate_ = pos; }

	void SetCoreWorldTransform(WorldTransform transform) { core_->GetWorldTransform() = transform; }

	void SetLeftWingWorldTransform(WorldTransform transform) { leftWing_->GetWorldTransform() = transform; }

	void SetRightWingWorldTransform(WorldTransform transform) { rightWing_->GetWorldTransform() = transform; }

	/// <summary>
	/// 移動アクティブフラグを設定
	/// </summary>
	/// <param name="flag">移動アクティブフラグ</param>
	void SetIsMoveActive(bool flag) { isMoveActive_ = flag; }
};
#pragma once

#include <Object/3D/Object3D.h>
#include "3d/Particle/EmitterGroup.h"
#include "3d/Camera/Camera.h"
#include <Scene/EngineContext.h>

#include <Object/Shadow/Shadow.h>

#include "3d/Collider/AABBCollider.h"
#include "3d/Collider/SphereCollider.h"

#include <Object/Enemy/State/EnemyEntryState.h>
#include <Object/Enemy/State/EnemyMoveState.h>
#include <Object/Enemy/State/EnemyAttackState.h>
#include <Object/Enemy/State/EnemyExitState.h>
#include <Object/Enemy/State/EnemyDeadState.h>

#include "memory"

/// === 前方宣言 === ///

class BulletManager;

class Player;

/// <summary>
/// 敵キャラクターを管理するクラスです。
/// </summary>
class Enemy {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 敵オブジェクトを初期化し、必要なパラメータや関連リソースを設定します。
	/// </summary>
	/// <param name="cameraPtr">追従対象のカメラを指定します。</param>
	/// <param name="bulletPtr">弾管理クラスのポインタを指定します。</param>
	/// <param name="playerPtr">プレイヤーオブジェクトのポインタを指定します。</param>
	/// <param name="objectData">敵オブジェクト生成に必要なデータを指定します。</param>
	void Initialize(EngineContext context, BulletManager* bulletPtr, Player* playerPtr, ObjectData objectData);

	/// <summary>
	/// 敵オブジェクトの状態を毎フレーム更新します。
	/// </summary>
	void Update();

	/// <summary>
	/// 敵オブジェクトと影オブジェクトの座標情報のみを更新します。
	/// </summary>
	void TransformUpdate();

	/// <summary>
	/// クリア時の敵オブジェクトの移動・回転処理を更新します。
	/// </summary>
	void ClearUpdate();

	/// <summary>
	/// 敵オブジェクトおよび関連する要素の描画処理を行います。
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵のクリア時爆発エミッターを起動します。
	/// </summary>
	void EmitClearExplosive();

	/// <summary>
	/// 死亡状態のチェック
	/// </summary>
	/// <returns> HPが死亡状態のHP以上であれば true : 死亡状態のHP以下であれば false</returns>
	bool CheckIsDead() { return hp_ >= deadHp_; }

	void ChangeState(std::unique_ptr<EnemyState> state);

	void StartBlink();

	void StartRemove();

	void StartDestroy();

	void ResetColor();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 他オブジェクトの接触時の処理を行います。
	/// </summary>
	void IsCollision();

	/// <summary>
	/// 敵オブジェクトの点滅処理を行います。
	/// </summary>
	void Blink();

	/// <summary>
	/// 敵オブジェクトの被弾時のシェイク処理を行います。
	/// </summary>
	void Shake();

	/// <summary>
	/// 敵のHPを確認し、死亡状態に遷移させる処理を行います。
	/// </summary>
	void CheckDead();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	MyEngine::Camera* camera_;

	//バレットマネージャー
	BulletManager* bulletManager_;

	//プレイヤー
	Player* player_;

	//オブジェクト
	std::unique_ptr<MyEngine::Object3D> object_ = nullptr;

	std::unique_ptr<EnemyState> state_;

	//コライダー
	std::unique_ptr<MyEngine::SphereCollider> collider_ = nullptr;

	//死亡時エミッター
	std::unique_ptr<MyEngine::EmitterGroup> explosiveEmitter_;

	//クリア時爆発エミッター
	std::unique_ptr<MyEngine::EmitterGroup> clearExplosiveEmitter_;

	//破壊時エミッター
	std::unique_ptr<MyEngine::EmitterGroup> destroyEmitter_;

	//ダメージ時エミッター
	std::unique_ptr<MyEngine::EmitterGroup> damageEmitter_;

	//影オブジェクト
	std::unique_ptr<Shadow> shadow_;

	//前フレームの座標
	MyEngine::Vector3 prePos_;

	//エントリー状態の座標
	MyEngine::Vector3 entryPos_;

	//スタンバイ状態の座標
	MyEngine::Vector3 standbyPos_;

	//離脱開始座標
	MyEngine::Vector3 exitStartPos_;

	//離脱座標
	MyEngine::Vector3 exitPos_;

	//死亡時移動量
	MyEngine::Vector3 deadMoveVelocity_;

	//体力
	int hp_;

	//死亡時に設定するHP
	int deadHp_;

	//弱攻撃ヒット時のダメージ
	int lightAttackDamage_;

	//中攻撃ヒット時のダメージ
	int mediumAttackDamage_;

	//点滅タイマー
	float blinkTimer_;

	//点滅間隔
	float blinkFrequency_;

	//被弾揺れタイマー
	float shakeTimer_;

	//被弾揺れ最大時間
	float shakeMaxTime_;

	//被弾揺れの長さ
	float shakeLength_;

	//画面揺れの時間
	float displayShakeTime_;

	//画面揺れの長さ
	float displayShakeLength_;

	//削除可能フラグ
	bool canRemove_;

	//削除中フラグ
	bool isRemove_;

	//透明化フラグ
	bool isInvisible_;

	bool isBlink_;

	///-------------------------------------------///
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// 座標のセッター
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(MyEngine::Vector3 pos) { object_->GetWorldTransform().translate_ = pos; }

	MyEngine::WorldTransform GetWorldTransform() { return object_->GetWorldTransform(); }

	void SetWorldTransform(MyEngine::WorldTransform transform) { object_->GetWorldTransform() = transform; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	MyEngine::Vector3 GetWorldPos() { return object_->GetWorldTransform().GetWorldTranslate(); }

	MyEngine::Vector3 GetPrePos() { return prePos_; }

	MyEngine::Vector3 GetPlayerPos();

	/// <summary>
	/// エントリーの位置を設定します。
	/// </summary>
	/// <param name="pos">座標</param>
	void SetEntryPos(MyEngine::Vector3 pos) { entryPos_ = pos; }

	MyEngine::Vector3 GetEntryPos() { return entryPos_; }

	/// <summary>
	/// スタンバイ状態の座標のセッター
	/// </summary>
	/// <param name="pos">座標</param>
	void SetStandbyPos(MyEngine::Vector3 pos) { standbyPos_ = pos; }

	MyEngine::Vector3 GetStandbyPos() { return standbyPos_; }

	/// <summary>
	/// 離脱開始座標の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetExitStartPos(MyEngine::Vector3 pos) { exitStartPos_ = pos; }

	MyEngine::Vector3 GetExitStartPos() { return exitStartPos_; }

	/// <summary>
	/// 離脱状態の座標の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetExitPos(MyEngine::Vector3 pos) { exitPos_ = pos; }

	MyEngine::Vector3 GetExitPos() { return exitPos_; }

	/// <summary>
	/// 削除中フラグのセッター
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetIsRemove(bool flag) { isRemove_ = flag; }

	/// <summary>
	/// 削除可能フラグのゲッター
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetCanRemove() { return canRemove_; }

	/// <summary>
	/// 削除中フラグのゲッター
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsRemove() { return isRemove_; }

	bool GetIsDead();

	BulletManager* GetBulletManager();

};
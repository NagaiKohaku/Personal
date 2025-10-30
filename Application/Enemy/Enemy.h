#pragma once

#include "3d/Object/Object3D.h"
#include "3d/Particle/EmitterGroup.h"
#include "3d/Camera/Camera.h"

#include "Shadow/Shadow.h"

#include "3d/Collider/AABBCollider.h"
#include "3d/Collider/SphereCollider.h"

#include "memory"

/// === 前方宣言 === ///

class BulletManager;

class Player;

///=====================================================/// 
/// エネミークラス
///=====================================================///
class Enemy {

	///-------------------------------------------/// 
	/// 列挙型
	///-------------------------------------------///
private:

	//行動状態
	enum STATE {
		ENTRY,
		MOVE,
		ATTACK,
		EXIT,
		DEAD
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* cameraPtr, BulletManager* bulletPtr, Player* playerPtr, ObjectData objectData);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 衝突時処理
	/// </summary>
	void IsCollision();

	/// <summary>
	/// 死亡時処理
	/// </summary>
	void Dead();

	/// <summary>
	/// エントリー時処理
	/// </summary>
	void Entry();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();

	/// <summary>
	/// 離脱時処理
	/// </summary>
	void Exit();

	/// <summary>
	/// 点滅処理
	/// </summary>
	void Blink();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	Camera* camera_;

	//バレットマネージャー
	BulletManager* bulletManager_;

	//プレイヤー
	Player* player_;

	//オブジェクト
	std::unique_ptr<Object3D> object_ = nullptr;

	//コライダー
	std::unique_ptr<SphereCollider> collider_ = nullptr;

	//死亡時エミッター
	std::unique_ptr<EmitterGroup> explosiveEmitter_;

	std::unique_ptr<Shadow> shadow_;

	//行動状態
	STATE state_;

	//エントリー状態の座標
	Vector3 entryPos_;

	//スタンバイ状態の座標
	Vector3 standbyPos_;

	//離脱開始座標
	Vector3 exitStartPos_;

	//離脱座標
	Vector3 exitPos_;

	//前フレームの座標
	Vector3 prePos_;

	//移動目標座標
	Vector3 targetPos_;

	//回転目標角度
	Vector3 targetRot_;

	//削除可能フラグ
	bool canRemove_;

	//アニメーションタイマー
	float animTimer_;

	//攻撃タイマー
	float attackTimer_;

	//点滅タイマー
	float blinkTimer_;

	//エントリー時アニメーション終了時間
	float entryAnimMaxTime_;

	//移動時のアニメーション終了時間
	float moveAnimMaxTime_;

	//攻撃時のアニメーション終了時間
	float attackAnimMaxTime_;

	//攻撃間隔
	float attackFrequency_;

	//離脱時アニメーション終了時間
	float exitAnimMaxTime_;

	//死亡時アニメーション終了時間
	float deadAnimMaxTime_;

	//点滅フラグ
	bool isBlink_;

	//点滅間隔
	float blinkFrequency_;

	///-------------------------------------------///
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// 座標のセッター
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(Vector3 pos) { object_->GetWorldTransform().translate_ = pos; }

	/// <summary>
	/// エントリーの位置を設定します。
	/// </summary>
	/// <param name="pos">座標</param>
	void SetEntryPos(Vector3 pos) { entryPos_ = pos; }

	/// <summary>
	/// スタンバイ状態の座標のセッター
	/// </summary>
	/// <param name="pos">座標</param>
	void SetStandbyPos(Vector3 pos) { standbyPos_ = pos; }

	/// <summary>
	/// 離脱開始座標の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetExitStartPos(Vector3 pos) { exitStartPos_ = pos; }

	/// <summary>
	/// 離脱状態の座標の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetExitPos(Vector3 pos) { exitPos_ = pos; }

	/// <summary>
	/// 削除可能フラグのゲッター
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetCanRemove() { return canRemove_; }

};
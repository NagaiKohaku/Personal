#pragma once

#include "3d/Object/Object3D.h"
#include "3d/Particle/EmitterGroup.h"
#include "3d/Camera/Camera.h"

#include "3d/Collider/AABBCollider.h"
#include "3d/Collider/SphereCollider.h"

#include "memory"

///=====================================================/// 
/// エネミークラス
///=====================================================///
class Enemy {

private:

	//行動状態
	enum STATE {
		ENTRY,
		STANDBY,
		MOVE,
		EXIT,
		DEAD
	};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* ptr);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

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
	/// 離脱時処理
	/// </summary>
	void Exit();

private:

	//カメラ
	Camera* camera_;

	//オブジェクト
	std::unique_ptr<Object3D> object_ = nullptr;

	//コライダー
	std::unique_ptr<SphereCollider> collider_ = nullptr;

	//死亡時エミッター
	std::unique_ptr<EmitterGroup> explosiveEmitter_;

	//行動状態
	STATE state_;

	//スタンバイ状態の座標
	Vector3 standbyPos_;

	//離脱座標
	Vector3 exitPos_;

	//削除可能フラグ
	bool canRemove_;

	//アニメーションタイマー
	float animTimer_;

	//エントリー時アニメーション終了時間
	float entryAnimMaxTime_;

	//離脱時アニメーション終了時間
	float exitAnimMaxTime_;

	//死亡時アニメーション終了時間
	float deadAnimMaxTime_;

public:

	/// <summary>
	/// 座標のセッター
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(Vector3 pos) { object_->GetWorldTransform().translate_ = pos; }

	/// <summary>
	/// スタンバイ状態の座標のセッター
	/// </summary>
	/// <param name="pos">座標</param>
	void SetStandbyPos(Vector3 pos) { standbyPos_ = pos; }

	/// <summary>
	/// 削除可能フラグのゲッター
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetCanRemove() { return canRemove_; }

};
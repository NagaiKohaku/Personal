#pragma once

#include "3d/Object/Object3D.h"
#include "3d/Object/DebugObject3D.h"
#include "3d/Object/DebugLine.h"

#include "Math/WorldTransform.h"
#include "Math/AABB.h"
#include "Math/Sphere.h"

#include "vector"
#include "memory"

///=====================================================/// 
/// コライダークラス
///=====================================================///
class Collider {

public:

	//識別タグ
	enum Tag {
		PLAYER,
		ENEMY,
		PLAYERBULLET,
		ENEMYBULLET,
		WALL,
		ITEM,
		NONE
	};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="parent">親オブジェクトのワールドトランスフォーム</param>
	virtual void Initialize(WorldTransform* parent);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

public:

	/// <summary>
	/// ワールドトランスフォームのゲッター
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// 識別タグのゲッター
	/// </summary>
	/// <returns>タグ</returns>
	Tag GetTag() const { return tag_; }

	/// <summary>
	/// 衝突相手の識別タグのゲッター
	/// </summary>
	/// <returns>タグ</returns>
	Tag GetHitTag() const { return hitTag_; }

	/// <summary>
	/// アクティブフラグのゲッター
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsActive() const { return isActive_; }

	/// <summary>
	/// 接触フラグのゲッター
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsCollision() const { return isCollision_; }

	/// <summary>
	/// トリガーフラグのゲッター
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsTrigger() const { return isTrigger_; }

	/// <summary>
	/// 識別タグのセッター
	/// </summary>
	/// <param name="tag">タグ</param>
	void SetTag(Tag tag) { tag_ = tag; }

	/// <summary>
	/// 接触相手の識別タグのセッター
	/// </summary>
	/// <param name="hitTag">タグ</param>
	void SetHitTag(Tag hitTag) { hitTag_ = hitTag; }

	/// <summary>
	/// 描画フラグのセッター
	/// </summary>
	/// <param name="isDraw">フラグ</param>
	void SetIsDraw(bool isDraw) { isDraw_ = isDraw; }

	/// <summary>
	/// アクティブフラグのセッター
	/// </summary>
	/// <param name="isActive">フラグ</param>
	void SetIsActive(bool isActive) { isActive_ = isActive; }

protected:

	//識別タグ
	Tag tag_ = NONE;

	//衝突相手の識別タグ
	Tag hitTag_ = NONE;

	//1フレーム前の衝突相手の識別タグ
	Tag hitTagBefore_ = NONE;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//親オブジェクトのワールドトランスフォーム
	WorldTransform* parentTransform_;

	//デバッグオブジェクト
	std::unique_ptr<DebugObject3D> debugObject_;

	//通常カラー
	Vector4 defaultColor_;

	//衝突時カラー
	Vector4 hitColor_;

	//描画フラグ
	bool isDraw_ = false;

	//アクティブフラグ
	bool isActive_ = true;

	//接触フラグ
	bool isCollision_ = false;

	//トリガーフラグ
	bool isTrigger_ = false;
};
#pragma once

#include "Collider.h"

#include <Math/Shape/AABB.h>

/// <summary>
/// AABBコライダー
/// </summary>
class AABBCollider : public Collider {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// AABBCollider を初期化します。
	/// </summary>
	/// <param name="parent"> コライダーの親ワールドトランスフォーム </param>
	void Initialize(WorldTransform* parent) override;

	/// <summary>
	/// AABBCollider を更新します。
	/// </summary>
	void Update() override;

	/// <summary>
	/// AABBCollider を描画します。
	/// </summary>
	void Draw() override;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//AABB情報
	AABB aabb_;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// AABBを取得
	/// </summary>
	/// <returns>AABB</returns>
	AABB GetAABB() const { return aabb_; }

};
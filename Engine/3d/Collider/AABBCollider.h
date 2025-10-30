#pragma once

#include "Collider.h"

#include "Math/AABB.h"

/// <summary>
/// AABBコライダー
/// </summary>
/// <remarks>
/// - 3Dオブジェクトの衝突判定に使用
/// - 基底クラス Collider を継承しています。
/// - 立方体（AABB）として表現され、最小座標と最大座標で衝突範囲を持ちます。
/// </remarks>
class AABBCollider : public Collider {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// AABBCollider を初期化します。
	/// </summary>
	/// <param name="parent"> コライダーの親ワールドトランスフォーム </param>
	/// <remarks>
	/// - 基底クラス Collider の初期化を行います。
	/// - AABB の最小値・最大値を設定します。
	/// - AABB の中心座標を設定します。
	/// - AABB のサイズに応じてスケールを設定します。
	/// - ColliderManager に自身を登録します。
	/// </remarks>
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
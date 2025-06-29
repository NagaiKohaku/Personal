#pragma once

#include "Collider.h"

#include "Math/AABB.h"

///=====================================================/// 
/// AABBコライダークラス
///=====================================================///
class AABBCollider : public Collider {

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="parent">親オブジェクトのワールドトランスフォーム</param>
	void Initialize(WorldTransform* parent) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

public:

	/// <summary>
	/// AABBのゲッター
	/// </summary>
	/// <returns>AABB</returns>
	AABB GetAABB() const { return aabb_; }

private:

	//AABB情報
	AABB aabb_;
};
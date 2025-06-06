#pragma once

#include "Collider.h"

#include "Math/AABB.h"

class AABBCollider : public Collider {

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform& parent) override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

public:

	AABB GetAABB() const { return aabb_; }

private:

	/// <summary>
	/// デバッグラインの生成
	/// </summary>
	void CreateDebugLines() override;

private:

	AABB aabb_; ///< AABB情報
};
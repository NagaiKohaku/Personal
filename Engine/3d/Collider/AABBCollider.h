#pragma once

#include "Collider.h"

#include "Math/AABB.h"

///=====================================================/// 
/// AABBコライダークラス
///=====================================================///
class AABBCollider : public Collider {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="parent">親オブジェクトのワールドトランスフォーム</param>
	void Initialize(WorldTransform* parent) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
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
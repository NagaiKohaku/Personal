#pragma once

#include "Collider.h"

#include "3d/Primitive/Ball.h"

#include "Math/Sphere.h"

///=====================================================/// 
/// 球体コライダー
///=====================================================///
class SphereCollider : public Collider{

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

	/// <summary>
	/// 削除処理
	/// </summary>
	void Remove();

public:

	/// <summary>
	/// Sphereのゲッター
	/// </summary>
	/// <returns>Sphere</returns>
	Sphere GetSphere() const { return sphere_; }

	/// <summary>
	/// Sphereの中心のセッター
	/// </summary>
	/// <param name="center">座標</param>
	void SetCenter(Vector3 center) { sphere_.center = center; }

	/// <summary>
	/// Sphereの大きさのセッター
	/// </summary>
	/// <param name="radius">半径</param>
	void SetRadius(float radius) { sphere_.radius = radius; }

private:

	//Sphere情報
	Sphere sphere_;

};
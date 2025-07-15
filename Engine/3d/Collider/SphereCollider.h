#pragma once

#include "Collider.h"

#include "3d/Mesh/SphereMesh.h"

#include "Math/Sphere.h"

///=====================================================/// 
/// 球体コライダークラス
///=====================================================///
class SphereCollider : public Collider{

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

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//Sphere情報
	Sphere sphere_;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// Sphereを取得
	/// </summary>
	/// <returns>Sphere</returns>
	Sphere GetSphere() const { return sphere_; }

	/// <summary>
	/// Sphereの中心の設定
	/// </summary>
	/// <param name="center">座標</param>
	void SetCenter(Vector3 center) { sphere_.center = center; }

	/// <summary>
	/// Sphereの大きさの設定
	/// </summary>
	/// <param name="radius">半径</param>
	void SetRadius(float radius) { sphere_.radius = radius; }
};
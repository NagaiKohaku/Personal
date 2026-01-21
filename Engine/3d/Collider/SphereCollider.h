#pragma once

#include "Collider.h"

#include "3d/Mesh/SphereMesh.h"

#include "Math/Sphere.h"

/// <summary>
/// 球状のコライダーを管理するクラスです。
/// </summary>
class SphereCollider : public Collider{

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// SphereCollider を初期化します。
	/// </summary>
	/// <param name="parent">コライダーの親ワールドトランスフォーム</param>
	void Initialize(WorldTransform* parent) override;

	/// <summary>
	/// SphereCollider を毎フレーム更新します。
	/// </summary>
	void Update() override;

	/// <summary>
	/// SphereCollider を描画します。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// SphereCollider をコライダーマネージャーから削除します。
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
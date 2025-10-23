#pragma once

#include "Collider.h"

#include "3d/Mesh/SphereMesh.h"

#include "Math/Sphere.h"

/// <summary>
/// 球状のコライダーを管理するクラスです。
/// </summary>
/// <remarks>
/// - 3Dオブジェクトの衝突判定に使用
/// - 基底クラス Collider を継承しています。
/// - 球体（Sphere）として表現され、中心座標と半径で衝突範囲を持ちます。
/// </remarks>
class SphereCollider : public Collider{

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// SphereCollider を初期化します。
	/// </summary>
	/// <param name="parent">コライダーの親ワールドトランスフォーム</param>
	/// <remarks>
	/// - 基底クラス Collider の初期化を行います。
	/// - 球の中心座標を設定します。
	/// - ワールドトランスフォームに球の中心を設定します。
	/// - ワールドトランスフォームに球の半径を設定します。
	/// - ModelManager を使用して球体モデルを作成します。
	/// - 球体モデルを設定します。
	/// - 親ワールドトランスフォームを設定します。
	/// - ColliderManager に自身を登録します。
	/// </remarks>
	void Initialize(WorldTransform* parent) override;

	/// <summary>
	/// SphereCollider を毎フレーム更新します。
	/// </summary>
	/// <remarks>
	/// - 球の中心座標をワールドトランスフォームに反映します。
	/// - 球の半径をワールドトランスフォームに反映します。
	/// - 基底クラス Collider の Update() を呼び出して衝突判定情報などを更新します。
	/// </remarks>
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
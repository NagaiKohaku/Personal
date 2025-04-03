#pragma once

#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"

///=====================================================/// 
/// ワールドトランスフォーム
///=====================================================///
class WorldTransform {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 行列の更新
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// パラメータのデバッグ表示
	/// </summary>
	void DisplayImGui();

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	const Vector3& GetForward() const;

	const Vector3& GetUp() const;

	const Vector3& GetRight() const;

	/// <summary>
	/// ワールド行列のゲッター
	/// </summary>
	/// <returns>ワールド行列</returns>
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }

	/// <summary>
	/// ワールド座標のゲッター
	/// </summary>
	/// <returns>ワールド座標</returns>
	const Vector3& GetWorldTranslate() const;

	/// <summary>
	/// ワールド行列のセッター
	/// </summary>
	/// <param name="matrix">ワールド行列</param>
	void SetWorldMatrix(const Matrix4x4& matrix) { worldMatrix_ = matrix; }

	/// <summary>
	/// オフセットのセッター
	/// </summary>
	/// <param name="vector">オフセット</param>
	void SetOffset(const Vector3& vector) { offset_ = vector; }

	/// <summary>
	/// 親オブジェクトのセッター
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(WorldTransform* parent);

public:

	//座標
	Vector3 translate_;

	//角度
	Vector3 rotate_;

	//大きさ
	Vector3 scale_;

private:

	//ワールド行列
	Matrix4x4 worldMatrix_;

	//オフセット
	Vector3 offset_;

	//親オブジェクト
	WorldTransform* parent_;
};
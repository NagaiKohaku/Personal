#pragma once

#include <Math/Vector/Vector3.h>
#include <Math/Matrix/Matrix4x4.h>

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

	/// === ベクトル取得 === ///

	/// <summary>
	/// 前方向のベクトルを取得
	/// </summary>
	/// <returns>ベクトル</returns>
	const Vector3 GetForward() const;

	/// <summary>
	/// 上方向のベクトルを取得
	/// </summary>
	/// <returns>ベクトル</returns>
	const Vector3 GetUp() const;

	/// <summary>
	/// 右方向のベクトルを取得
	/// </summary>
	/// <returns>ベクトル</returns>
	const Vector3 GetRight() const;

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	/// <returns>ベクトル</returns>
	const Vector3 GetWorldTranslate() const;

	/// <summary>
	/// ワールド回転の取得
	/// </summary>
	/// <returns>ベクトル</returns>
	const Vector3 GetWorldRotate() const;

	/// <summary>
	/// ワールド拡大縮小の取得
	/// </summary>
	/// <returns>ベクトル</returns>
	const Vector3 GetWorldScale() const;

	/// === 行列取得 === ///

	/// <summary>
	/// ワールド行列を取得
	/// </summary>
	/// <returns>行列</returns>
	const Matrix4x4 GetWorldMatrix() const { return worldMatrix_; }

	/// <summary>
	/// ワールド座標変換行列の取得
	/// </summary>
	/// <returns>行列</returns>
	const Matrix4x4 GetWorldTranslateMatrix() const;

	/// <summary>
	/// ワールド回転行列の取得
	/// </summary>
	/// <returns>行列</returns>
	const Matrix4x4 GetWorldRotateMatrix() const;

	/// <summary>
	/// ワールド拡大縮小行列の取得
	/// </summary>
	/// <returns>行列</returns>
	const Matrix4x4 GetWorldScaleMatrix() const;

	/// <summary>
	/// ローカル座標変換行列を取得
	/// </summary>
	/// <returns>行列</returns>
	const Matrix4x4 GetLocalTranslateMatrix() const;

	/// <summary>
	/// ローカル回転行列を取得
	/// </summary>
	/// <returns>行列</returns>
	const Matrix4x4 GetLocalRotateMatrix() const;

	/// <summary>
	/// ローカル拡大縮小行列を取得
	/// </summary>
	/// <returns></returns>
	const Matrix4x4 GetLocalScaleMatrix() const;

	/// <summary>
	/// ワールド行列の設定
	/// </summary>
	/// <param name="matrix">行列</param>
	void SetWorldMatrix(const Matrix4x4& matrix) { worldMatrix_ = matrix; }

	/// <summary>
	/// 回転行列の設定
	/// </summary>
	/// <param name="matrix">行列</param>
	void SetRotateMatrix(const Matrix4x4& matrix){}

	/// <summary>
	/// オフセットの設定
	/// </summary>
	/// <param name="vector">オフセット</param>
	void SetOffset(const Vector3& vector) { offset_ = vector; }

	/// <summary>
	/// 親オブジェクトの設定
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

	//ローカル行列
	Matrix4x4 localMatrix_;

	//座標変換行列
	Matrix4x4 translateMatrix_;

	//回転行列
	Matrix4x4 rotateMatrix_;

	//拡大縮小行列
	Matrix4x4 scaleMatrix_;

	//オフセット
	Vector3 offset_;

	//親オブジェクト
	WorldTransform* parent_;
};
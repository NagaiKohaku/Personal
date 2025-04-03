#pragma once
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "Math/WorldTransform.h"

class Object3D;

///=====================================================/// 
/// カメラ
///=====================================================///
class Camera {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// 追従対象のセッター
	/// </summary>
	/// <param name="object">3Dオブジェクト</param>
	void SetTrackingObject(Object3D* object) { object_ = object; }

	/// <summary>
	/// 追従対象のリセット
	/// </summary>
	void ResetTrackingObject() { object_ = nullptr; }

	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

	/// <summary>
	/// fovYのセッター
	/// </summary>
	/// <param name="fovY">fovY</param>
	void SetFovY(const float fovY) { fovY_ = fovY; }

	/// <summary>
	/// アスペクト比のセッター
	/// </summary>
	/// <param name="aspectRatio">アスペクト比</param>
	void SetAspectRatio(const float aspectRatio) { aspectRatio_ = aspectRatio; }

	/// <summary>
	/// NearClipのセッター
	/// </summary>
	/// <param name="nearClip">NearClip</param>
	void SetNearClip(const float nearClip) { nearClip_ = nearClip; }

	/// <summary>
	/// FarClipのセッター
	/// </summary>
	/// <param name="farClip">FarClip</param>
	void SetFarClip(const float farClip) { farClip_ = farClip; }

	/// <summary>
	/// デバッグカメラフラグのセッター
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetDebugCameraFlag(const bool flag) { isDebugCamera_ = flag; }

	/// <summary>
	/// ワールドトランスフォームのゲッター
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	WorldTransform& GetWorldTransform() { return isDebugCamera_ ? debugTransform_ : transform_; }

	/// <summary>
	/// ビュー行列のゲッター
	/// </summary>
	/// <returns>ビュー行列</returns>
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }

	/// <summary>
	/// プロジェクション行列のゲッター
	/// </summary>
	/// <returns>プロジェクション行列</returns>
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }

	/// <summary>
	/// ビュープロジェクション行列のゲッター
	/// </summary>
	/// <returns>ビュープロジェクション行列</returns>
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix_; }

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
private:

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//追従対象
	Object3D* object_;

	//座標
	WorldTransform transform_;

	//デバッグカメラの座標
	WorldTransform debugTransform_;

	//ビュー行列
	Matrix4x4 viewMatrix_;

	//プロジェクション行列
	Matrix4x4 projectionMatrix_;

	//ビュープロジェクション行列
	Matrix4x4 viewProjectionMatrix_;

	//視野角
	float fovY_;

	//アスペクト比
	float aspectRatio_;

	//NearClip
	float nearClip_;

	//FarClip
	float farClip_;

	//Z軸のオフセット
	float offsetZ_;

	//デバッグカメラのZ軸のオフセット
	float debugCameraOffsetZ_;

	//デバッグカメラフラグ
	bool isDebugCamera_;

};
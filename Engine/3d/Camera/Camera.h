#pragma once
#include "3d/Object/Object3D.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "Math/WorldTransform.h"

///=====================================================/// 
/// カメラクラス
///=====================================================///
class Camera {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// カメラの各種パラメータを初期化します。
	/// </summary>
	/// <remarks>
	/// 座標、ビュー・プロジェクション行列、クリップ距離、視野角、アスペクト比などを設定します。
	/// また、デバッグカメラや関連描画モジュールにこのカメラをデフォルトとして登録します。
	/// </remarks>
	void Initialize();

	/// <summary>
	/// カメラの状態を更新します。
	/// </summary>
	/// <remarks>
	/// 通常カメラまたはデバッグカメラのどちらを使用するかを判定し、
	/// 現在の入力状況に応じてカメラの位置や回転を更新します。
	/// デバッグカメラ状態ではマウス操作による移動・回転・ズームが可能です。
	/// 通常カメラ状態では、追従対象が存在する場合にそのオブジェクトへ追従します。
	/// 各モードの更新後、ビュー行列・プロジェクション行列・ビュープロジェクション行列を再計算します。
	/// </remarks>
	void Update();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// 追従対象のリセット
	/// </summary>
	void ResetTrackingObject() { trackingObject_ = nullptr; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//追従対象
	Object3D* trackingObject_;

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

	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	WorldTransform& GetWorldTransform() { return isDebugCamera_ ? debugTransform_ : transform_; }

	/// <summary>
	/// ビュー行列を取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }

	/// <summary>
	/// プロジェクション行列を取得
	/// </summary>
	/// <returns>プロジェクション行列</returns>
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }

	/// <summary>
	/// ビュープロジェクション行列を取得
	/// </summary>
	/// <returns>ビュープロジェクション行列</returns>
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix_; }

	/// <summary>
	/// デバッグカメラフラグを取得
	/// </summary>
	/// <returns>フラグ</returns>
	bool IsDebugCamera() const { return isDebugCamera_; }

	/// <summary>
	/// fovYの設定
	/// </summary>
	/// <param name="fovY">fovY</param>
	void SetFovY(const float fovY) { fovY_ = fovY; }

	/// <summary>
	/// アスペクト比の設定
	/// </summary>
	/// <param name="aspectRatio">アスペクト比</param>
	void SetAspectRatio(const float aspectRatio) { aspectRatio_ = aspectRatio; }

	/// <summary>
	/// NearClipの設定
	/// </summary>
	/// <param name="nearClip">NearClip</param>
	void SetNearClip(const float nearClip) { nearClip_ = nearClip; }

	/// <summary>
	/// FarClipの設定
	/// </summary>
	/// <param name="farClip">FarClip</param>
	void SetFarClip(const float farClip) { farClip_ = farClip; }

	/// <summary>
	/// デバッグカメラフラグの設定
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetDebugCameraFlag(const bool flag) { isDebugCamera_ = flag; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetTransform(const Vector3 pos) { isDebugCamera_ ? debugTransform_.translate_ = pos : transform_.translate_ = pos; }

	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rot">角度</param>
	void SetRotate(const Vector3 rot) { isDebugCamera_ ? debugTransform_.rotate_ = rot : transform_.rotate_ = rot; }

	/// <summary>
	/// オフセットの設定
	/// </summary>
	/// <param name="offset">オフセット</param>
	void SetOffsetZ(const float offset) { isDebugCamera_ ? debugCameraOffsetZ_ = offset : offsetZ_ = offset; }

	/// <summary>
	/// 追従対象の設定
	/// </summary>
	/// <param name="object">3Dオブジェクト</param>
	void SetTrackingObject(Object3D* object) { trackingObject_ = object; }

};
#pragma once
#include <Base/OffScreen.h>
#include <3d/Object/Object3D.h>
#include <Math/Vector/Vector3.h>
#include <Math/Matrix/Matrix4x4.h>
#include <Math/Transform/WorldTransform.h>

#include <3d/Object/Object3DCommon.h>
#include <3d/Object/DebugObjectCommon.h>
#include <3d/Object/SkyBoxCommon.h>

namespace MyEngine {

	/// <summary>
	/// 3Dシーンの視点制御を行うカメラクラスです。
	/// </summary>
	class Camera {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// カメラの各種パラメータを初期化します。
		/// </summary>
		void Initialize(float windowWidth, float windowHeight, OffScreen* offScreenPtr, Object3DCommon* object3DCommonPtr, DebugObjectCommon* debugObjectCommonPtr, SkyBoxCommon* skyBoxCommonPtr);

		/// <summary>
		/// カメラの状態を更新します。
		/// </summary>
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

		OffScreen* offScreen_;

		//追従対象
		Object3D* trackingObject_;

		//座標
		WorldTransform transform_;

		//デバッグカメラの座標
		WorldTransform debugTransform_;

		//ビュー行列
		Matrix4x4 viewMatrix_;

		//ビューポート行列
		Matrix4x4 viewportMatrix_;

		//直交投影行列
		Matrix4x4 orthographicProjectionMatrix_;

		//透視投影行列
		Matrix4x4 perspectiveProjectionMatrix_;

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
		/// ビューポート行列を取得
		/// </summary>
		/// <returns>ビューポート行列</returns>
		const Matrix4x4& GetViewPortMatrix() const { return viewportMatrix_; }

		/// <summary>
		/// 透視投影行列を取得
		/// </summary>
		/// <returns>透視投影行列</returns>
		const Matrix4x4& Get3DProjectionMatrix() const { return perspectiveProjectionMatrix_; }

		/// <summary>
		/// 直交投影行列を取得
		/// </summary>
		/// <returns>直交投影行列</returns>
		const Matrix4x4& Get2DProjectionMatrix() const { return orthographicProjectionMatrix_; }

		/// <summary>
		/// ビュー透視投影行列を取得
		/// </summary>
		/// <returns>ビュー透視投影行列</returns>
		Matrix4x4 Get3DViewProjectionMatrix() const;

		/// <summary>
		/// ビュー直交投影行列を取得
		/// </summary>
		/// <returns>ビュー直交投影行列</returns>
		Matrix4x4 Get2DViewProjectionMatrix() const;

		/// <summary>
		/// デバッグカメラフラグを取得
		/// </summary>
		/// <returns>フラグ</returns>
		bool IsDebugCamera() const { return isDebugCamera_; }

		/// <summary>
		/// ビュー行列の設定
		/// </summary>
		/// <param name="matrix">ビュー行列</param>
		void SetViewMatrix(Matrix4x4 matrix) { viewMatrix_ = matrix; }

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
}
#include "Camera.h"

#include "Base/WinApp.h"
#include "Base/Input.h"
#include "Base/OffScreen.h"
#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugObjectCommon.h"
#include "3d/Object/SkyBoxCommon.h"

#include <Math/Utility/MakeMatrixMath.h>

#include <Effect/Shake/Shake.h>

#include "imgui.h"

#include "numbers"

namespace MyEngine {

	///=====================================================/// 
	/// カメラの各種パラメータを初期化
	///=====================================================///
	void Camera::Initialize(float windowWidth, float windowHeight) {

		//カメラの座標の設定
		transform_.Initialize();

		transform_.UpdateMatrix();

		//デバッグカメラの座標の設定
		debugTransform_.Initialize();

		debugTransform_.UpdateMatrix();

		//視野角の設定
		fovY_ = 0.45f;

		//アスペクト比の設定
		aspectRatio_ = windowWidth / windowHeight;

		//NearClipの設定
		nearClip_ = 0.1f;

		//FarClipの設定
		farClip_ = 1000.0f;

		//ビュー行列の生成
		viewMatrix_ = Inverse4x4(transform_.GetWorldMatrix());

		//ビューポート行列の生成
		viewportMatrix_ = MakeViewportMatrix(0, 0, windowWidth, windowHeight, 0, 1);

		//透視投影行列の生成
		perspectiveProjectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);

		//直交投影行列の生成
		orthographicProjectionMatrix_ = MakeOrthographicMatrix(0.0f, 0.0f, windowWidth, windowHeight, 0.0f, 100.0f);

		//Z軸のオフセットの設定
		offsetZ_ = 0.0f;

		//デバッグカメラのZ軸のオフセットの設定
		debugCameraOffsetZ_ = -15.0f;

		//デバッグカメラフラグの設定
		isDebugCamera_ = false;

		OffScreen::GetInstance()->SetDefaultCamera(this);

		Object3DCommon::GetInstance()->SetDefaultCamera(this);

		DebugObjectCommon::GetInstance()->SetDefaultCamera(this);

		SkyBoxCommon::GetInstance()->SetDefaultCamera(this);
	}

	///=====================================================/// 
	/// カメラの状態を更新
	///=====================================================///
	void Camera::Update() {

		if (Input::GetInstance()->isPushKey(DIK_LCONTROL)) {
			if (Input::GetInstance()->IsTriggerPushKey(DIK_Q)) {

				isDebugCamera_ = !isDebugCamera_;
			}
		}

		if (isDebugCamera_) {

			/// === デバッグカメラ状態の場合 === ///

			//今フレームの回転角度
			Vector3 rotateDelta = { 0.0f,0.0f,0.0f };

			//移動量
			Vector3 velocity = { 0.0f,0.0f,0.0f };

			//LShiftを押していたらカメラ操作
			if (Input::GetInstance()->isPushKey(DIK_LSHIFT)) {

				//左クリックしていたらカメラ座標の移動
				if (Input::GetInstance()->IsPushMouseButton(0)) {

					velocity.x = -Input::GetInstance()->GetMouseVelocity().x * 0.01f;
					velocity.y = Input::GetInstance()->GetMouseVelocity().y * 0.01f;
				}

				//右クリックしていたらカメラの回転
				if (Input::GetInstance()->IsPushMouseButton(1)) {

					rotateDelta.x = (static_cast<float>(std::numbers::pi) / 180.0f) * Input::GetInstance()->GetMouseVelocity().y;

					rotateDelta.y = (static_cast<float>(std::numbers::pi) / 180.0f) * Input::GetInstance()->GetMouseVelocity().x;
				}

				//カメラ座標Z軸の移動
				debugCameraOffsetZ_ += Input::GetInstance()->GetMouseVelocity().z * 0.01f;
			}

			/// === 角度の設定 === ///

			//今フレームの回転角度を加える
			debugTransform_.rotate_ += rotateDelta;

			//角度行列を生成
			Matrix4x4 matRot_ =
				(MakeRotateXMatrix(debugTransform_.rotate_.x) *
					MakeRotateYMatrix(debugTransform_.rotate_.y)) *
				MakeRotateZMatrix(debugTransform_.rotate_.z);

			/// === 座標の設定 === ///

			//追従対象からカメラまでのオフセット
			Vector3 offset = { 0.0f,0.0f,debugCameraOffsetZ_ };

			//オフセットをカメラの回転に合わせる
			offset = TransformNormal(offset, matRot_);

			//移動量をカメラの回転に合わせる
			velocity = TransformNormal(velocity, matRot_);

			debugTransform_.SetOffset(offset);

			//今フレームの移動量を加える
			debugTransform_.translate_ += velocity;

			/// === 行列の計算 === ///

			//ワールドトランスフォームの更新
			debugTransform_.UpdateMatrix();

			//ビュー行列の計算
			viewMatrix_ = Inverse4x4(debugTransform_.GetWorldMatrix());

		} else {

			/// === 通常カメラの場合 === ///

				//角度行列を生成
			Matrix4x4 matRot_ =
				(MakeRotateXMatrix(transform_.rotate_.x) *
					MakeRotateYMatrix(transform_.rotate_.y)) *
				MakeRotateZMatrix(transform_.rotate_.z);

			//追従対象からカメラまでのオフセット
			Vector3 offset = { 0.0f,0.0f,offsetZ_ };

			//オフセットをカメラの回転に合わせる
			offset = TransformNormal(offset, matRot_);

			transform_.SetOffset(offset);

			//追従対象がいる場合
			if (trackingObject_) {

				/// === 追従カメラの場合 === ///

				//カメラ座標を追従対象を中心にオフセット分ずらす
				transform_.translate_ = trackingObject_->GetWorldTransform().GetWorldTranslate();
			}

			/// === 行列の計算 === ///

			//ワールド行列の計算
			transform_.UpdateMatrix();

			//ビュー行列の計算
			viewMatrix_ = Inverse4x4(transform_.GetWorldMatrix());

			Shake::GetInstance()->Update();
		}
	}

	///=====================================================/// 
	/// ImGuiの表示
	///=====================================================///
	void Camera::DisplayImGui() {

#ifdef _USE_IMGUI

		ImGui::Checkbox("DebugCamera", &isDebugCamera_);

		if (isDebugCamera_) {

			debugTransform_.DisplayImGui();

			ImGui::DragFloat("offsetZ", &debugCameraOffsetZ_, 0.1f);

		} else {

			transform_.DisplayImGui();

			ImGui::DragFloat("offsetZ", &offsetZ_, 0.1f);
		}

#endif // _USE_IMGUI
	}
	Matrix4x4 Camera::Get3DViewProjectionMatrix() const {

		Matrix4x4 viewProjectionMatrix = viewMatrix_ * perspectiveProjectionMatrix_;

		return viewProjectionMatrix;
	}
	Matrix4x4 Camera::Get2DViewProjectionMatrix() const {

		Matrix4x4 viewMatrix = MakeIdentity4x4();

		Matrix4x4 viewProjectionMatrix = viewMatrix * orthographicProjectionMatrix_;

		return viewProjectionMatrix;
	}
}
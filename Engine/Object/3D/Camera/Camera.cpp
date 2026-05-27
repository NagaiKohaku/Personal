#include "Camera.h"

#include <Math/Utility/MakeMatrixMath.h>

#include "imgui.h"
#include "numbers"

namespace MyEngine {

	///=====================================================/// 
	/// カメラの各種パラメータを初期化
	///=====================================================///
	void Camera::Initialize(uint32_t windowWidth, uint32_t windowHeight, DirectXCommon* directCommonPtr, Input* inputPtr) {

		float wWidth = static_cast<float>(windowWidth);
		float wHeight = static_cast<float>(windowHeight);

		directCommon_ = directCommonPtr;

		input_ = inputPtr;

		//カメラ情報のバッファリソースの生成
		cameraForGpuResource = directCommon_->CreateBufferResource(sizeof(CameraForGPU));

		//リソースにカメラ情報を記録
		cameraForGpuResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGpuData));

		//カメラ情報の設定
		cameraForGpuData->worldPosition = Vector3(0.0f, 0.0f, 0.0f);

		transform_.SetDirectCommon(directCommon_);

		//カメラの座標の設定
		transform_.Initialize();

		debugTransform_.SetDirectCommon(directCommon_);

		//デバッグカメラの座標の設定
		debugTransform_.Initialize();

		//視野角の設定
		fovY_ = 0.45f;

		//アスペクト比の設定
		aspectRatio_ = wWidth / wHeight;

		//NearClipの設定
		nearClip_ = 0.1f;

		//FarClipの設定
		farClip_ = 1000.0f;

		//ビュー行列の生成
		viewMatrix_ = Inverse4x4(transform_.GetWorldMatrix());

		//ビューポート行列の生成
		viewportMatrix_ = MakeViewportMatrix(0, 0, wWidth, wHeight, 0, 1);

		//透視投影行列の生成
		perspectiveProjectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);

		//直交投影行列の生成
		orthographicProjectionMatrix_ = MakeOrthographicMatrix(0.0f, 0.0f, wWidth, wHeight, 0.0f, 100.0f);

		//Z軸のオフセットの設定
		offsetZ_ = 0.0f;

		//デバッグカメラのZ軸のオフセットの設定
		debugCameraOffsetZ_ = -15.0f;

		//デバッグカメラフラグの設定
		isDebugCamera_ = false;

		transform_.Update();

		debugTransform_.Update();

	}

	///=====================================================/// 
	/// カメラの状態を更新
	///=====================================================///
	void Camera::Update() {

		if (input_->isPushKey(DIK_LCONTROL)) {
			if (input_->IsTriggerPushKey(DIK_Q)) {

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
			if (input_->isPushKey(DIK_LSHIFT)) {

				//左クリックしていたらカメラ座標の移動
				if (input_->IsPushMouseButton(0)) {

					velocity.x = -input_->GetMouseVelocity().x * 0.01f;
					velocity.y = input_->GetMouseVelocity().y * 0.01f;
				}

				//右クリックしていたらカメラの回転
				if (input_->IsPushMouseButton(1)) {

					rotateDelta.x = (static_cast<float>(std::numbers::pi) / 180.0f) * input_->GetMouseVelocity().y;

					rotateDelta.y = (static_cast<float>(std::numbers::pi) / 180.0f) * input_->GetMouseVelocity().x;
				}

				//カメラ座標Z軸の移動
				debugCameraOffsetZ_ += input_->GetMouseVelocity().z * 0.01f;
			}

			/// === 角度の設定 === ///

			//今フレームの回転角度を加える
			debugTransform_.SetRotate(rotateDelta);

			//角度行列を生成
			Matrix4x4 matRot_ = debugTransform_.GetLocalRotateMatrix();

			/// === 座標の設定 === ///

			//追従対象からカメラまでのオフセット
			Vector3 offset = { 0.0f,0.0f,debugCameraOffsetZ_ };

			//オフセットをカメラの回転に合わせる
			offset = TransformNormal(offset, matRot_);

			//移動量をカメラの回転に合わせる
			velocity = TransformNormal(velocity, matRot_);

			debugTransform_.SetOffset(offset);

			//今フレームの移動量を加える
			debugTransform_.SetTranslate(debugTransform_.GetTranslate() + velocity);

			/// === 行列の計算 === ///

			//ワールドトランスフォームの更新
			debugTransform_.Update();

			//ビュー行列の計算
			viewMatrix_ = Inverse4x4(debugTransform_.GetWorldMatrix());

		} else {

			/// === 通常カメラの場合 === ///

				//角度行列を生成
			Matrix4x4 matRot_ = transform_.GetLocalRotateMatrix();

			//追従対象からカメラまでのオフセット
			Vector3 offset = { 0.0f,0.0f,offsetZ_ };

			//オフセットをカメラの回転に合わせる
			offset = TransformNormal(offset, matRot_);

			transform_.SetOffset(offset);

			/// === 行列の計算 === ///

			//ワールド行列の計算
			transform_.Update();

			//ビュー行列の計算
			viewMatrix_ = Inverse4x4(transform_.GetWorldMatrix());
		}

		//カメラ位置を取得
		cameraForGpuData->worldPosition = GetWorldTransform().GetWorldTranslate();
	}

	///=====================================================/// 
	/// GPUにディレクショナルライトのデータを送信
	///=====================================================///
	void Camera::SendDataForGPU() {

		//光源データをGPUに送信
		directCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, cameraForGpuResource.Get()->GetGPUVirtualAddress());
	}

	///=====================================================/// 
	/// ImGuiの表示
	///=====================================================///
	void Camera::DisplayImGui() {

#ifdef _USE_IMGUI

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
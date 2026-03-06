#pragma once

#include <Object/Base/ObjectCommonBase.h>
#include <3d/Light/DirectionalLight.h>
#include <3d/Light/PointLight.h>
#include <3d/Light/SpotLight.h>

namespace MyEngine {

	class Camera;

	class Object3DCommon : public ObjectCommonBase {

		///-------------------------------------------/// 
		/// 構造体
		///-------------------------------------------///
	private:

		struct CameraForGPU {
			Vector3 worldPosition;
		};

	public:

		Object3DCommon();

		~Object3DCommon();

	public:

		void Initialize(DirectXCommon* dxCommonPtr) override;

		void CommonDrawSetting() override;

		void CreateRootSignature() override;

		void CreateInputElement(
			std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElements
		) override;

		void CreateShader(
			Microsoft::WRL::ComPtr<IDxcBlob>& vs,
			Microsoft::WRL::ComPtr<IDxcBlob>& ps
		) override;

	private:

		//デフォルトカメラ
		Camera* defaultCamera_ = nullptr;

		//平行光源ライト
		std::unique_ptr<DirectionalLight> directionalLight_;

		//点光源ライト
		std::unique_ptr<PointLight> pointLight_;

		//スポットライト
		std::unique_ptr<SpotLight> spotLight_;

		//バッファリソース内のデータを指すポインタ
		CameraForGPU* cameraForGpuData = nullptr;

		//バッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> cameraForGpuResource = nullptr;

		//キューブテクスチャのファイルパス
		std::string textureCubeFilePath_;

	public:

		/// <summary>
		/// デフォルトカメラを取得
		/// </summary>
		/// <returns></returns>
		Camera* GetDefaultCamera() const { return defaultCamera_; }

		/// <summary>
		/// デフォルトカメラの設定
		/// </summary>
		/// <param name="camera">カメラ</param>
		void SetDefaultCamera(Camera* camera) { defaultCamera_ = camera; }

		/// <summary>
		/// キューブテクスチャのファイルパスの設定
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		void SetTextureCubeFilePath(const std::string& filePath) { textureCubeFilePath_ = filePath; }

	};
}
#pragma once

#include <Base/DirectXCommon.h>
#include <Math/Vector/Vector3.h>

namespace MyEngine {

	/// <summary>
	/// モデル共通処理を管理するシングルトンクラスです。
	/// </summary>
	class ModelCommon {

		///-------------------------------------------/// 
		/// 構造体
		///-------------------------------------------///
	private:

		struct CameraForGPU {
			Vector3 worldPosition;
		};

	public:

		ModelCommon();

		~ModelCommon();

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// ModelCommonの初期化を行います。
		/// </summary>
		void Initialize(DirectXCommon* dxCommonPtr);

		/// <summary>
		/// 3Dオブジェクトの描画に必要な共通設定を行います。
		/// </summary>
		void CommonDrawSetting();

		///-------------------------------------------/// 
		/// クラス内処理関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// 3Dオブジェクト描画用のルートシグネチャを作成します。
		/// </summary>
		void CreateRootSignature();

		/// <summary>
		/// 3Dオブジェクト描画用のグラフィックパイプラインステートを作成します。
		/// </summary>
		void CreateGraphicsPipeline();

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//DirectX基底
		DirectXCommon* dxCommon_;

		//バッファリソース内のデータを指すポインタ
		CameraForGPU* cameraForGpuData_ = nullptr;

		//バッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> cameraForGpuResource_ = nullptr;

		//ルートシグネチャ
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

		//グラフィックパイプラインステート
		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;

		///-------------------------------------------/// 
		/// ゲッター・セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// DirectX基底の取得
		/// </summary>
		/// <returns>DirectX基底</returns>
		DirectXCommon* GetDxCommon() const { return dxCommon_; }

	};
}
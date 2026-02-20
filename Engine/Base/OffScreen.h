#pragma once

#include "d3d12.h"

#include <Math/Vector/Vector4.h>
#include <Math/Matrix/Matrix4x4.h>

#include "wrl.h"
#include "vector"
#include "cstdint"
#include "string"

namespace MyEngine {

	/// === 前方宣言 === ///

	class WinApp;

	class DirectXCommon;

	class RTVManager;

	class DSVManager;

	class SrvManager;

	class Camera;

	/// <summary>
	/// オフスクリーン描画用のレンダーテクスチャや深度バッファを管理し、描画結果をスワップチェーンに転送できるクラスです。
	/// </summary>
	class OffScreen {

		///-------------------------------------------/// 
		/// メンバ構造体
		///-------------------------------------------///
	private:

		//マテリアル
		struct Material {
			Matrix4x4 projectionInverse;
			float colorReverseRatio;
		};

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// OffScreenのシングルトンインスタンスを取得します。
		/// </summary>
		static OffScreen* GetInstance();

		/// <summary>
		/// オフスクリーン描画用のリソースやパイプラインを初期化します。
		/// </summary>
		void Initialize(WinApp* winAppPtr, DirectXCommon* dxCommonPtr);

		/// <summary>
		/// オフスクリーン描画用のレンダーターゲットと深度バッファの描画準備を行います。
		/// </summary>
		void PreDraw();

		/// <summary>
		/// オフスクリーン描画後にレンダーターゲットと深度バッファをシェーダーで使用できる状態に戻します。
		/// </summary>
		void PostDraw();

		/// <summary>
		/// オフスクリーンで描画した内容をスワップチェーンのバックバッファに転送して描画します。
		/// </summary>
		void DrawToSwapChain();

		/// <summary>
		/// ImGuiの表示
		/// </summary>
		void ImGui();

		///-------------------------------------------/// 
		/// クラス内処理関数
		///-------------------------------------------///
	private:

		/// <summary>
		/// オフスクリーン描画用のルートシグネチャを作成します。
		/// </summary>
		void CreateRootSignature();

		/// <summary>
		/// オフスクリーン描画用のグラフィックスパイプラインを生成します。
		/// </summary>
		void CreatePipeline();

		/// <summary>
		/// オフスクリーン用のRenderTargetView(RTV)を生成します。
		/// </summary>
		void CreateRenderTargetView();

		/// <summary>
		/// オフスクリーン用のDepthStencilView(DSV)を生成します。
		/// </summary>
		void CreateDepthStencilView();

		/// <summary>
		/// オフスクリーン用のレンダーテクスチャSRVを生成します。
		/// </summary>
		void CreateRenderTextureSRV();

		/// <summary>
		/// 深度テクスチャ用のSRVを生成します。
		/// </summary>
		void CreateDepthTextureSRV();

		/// <summary>
		/// 指定サイズ・フォーマットのレンダーターゲット用テクスチャを生成します。
		/// </summary>
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTexture(
			Microsoft::WRL::ComPtr<ID3D12Device> device,
			uint32_t width,
			uint32_t height,
			DXGI_FORMAT format,
			const Vector4& clearColor
		);

		/// <summary>
		/// 指定サイズ・フォーマットの深度ステンシル用テクスチャを生成します。
		/// </summary>
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthTexture(
			Microsoft::WRL::ComPtr<ID3D12Device> device,
			uint32_t width,
			uint32_t height,
			DXGI_FORMAT format
		);

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//ウィンドウクラス
		WinApp* winApp_ = nullptr;

		//DirectX基底
		DirectXCommon* dxCommon_ = nullptr;

		//RTVマネージャー
		RTVManager* rtvManager_ = nullptr;

		//DSVマネージャー
		DSVManager* dsvManager_ = nullptr;

		//SRVマネージャー
		SrvManager* srvManager_ = nullptr;

		//カメラ
		Camera* camera_ = nullptr;

		//オフスクリーン用のテクスチャ
		Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResrouce_;

		//DepthTextreのリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> depthTextureResource_;

		//オフスクリーン用のマテリアルリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;

		//RTVハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE offScreenRTVHandle_;

		//RTVのメモリ番号
		uint32_t rtvIndex_ = 0;

		//DSVハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE offScreenDSVHandle_;

		//DSVのメモリ番号
		uint32_t dsvIndex_ = 0;

		//renderTextureハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE renderTextureSRVHandle_;

		//renderTextureのメモリ番号
		uint32_t renderTextureSRVIndex_ = 0;

		//DepthTextureハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE depthTextureSRVHandle_;

		//DepthTextureのメモリ番号
		uint32_t depthTextureSRVIndex_ = 0;

		//オフスクリーン用のマテリアルデータ
		Material* materialData_ = nullptr;

		//ルートシグネチャ
		Microsoft::WRL::ComPtr<ID3D12RootSignature> offScreenRootSignature_ = nullptr;

		//グラフィックパイプラインステート
		std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> offScreenGraphicsPipelineState_;

		//現在のシェーダー名
		std::wstring currentShaderName_;

		//オフスクリーンのクリア値
		Vector4 offScreenClearColor_ = { 0.0f,0.0f,0.0f,1.0f };

		///-------------------------------------------/// 
		/// ゲッター・セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// デフォルトカメラの設定
		/// </summary>
		/// <param name="ptr">カメラポインタ</param>
		void SetDefaultCamera(Camera* ptr);

		/// <summary>
		/// 色反転係数の設定
		/// </summary>
		/// <param name="num">色反転係数</param>
		void SetColorReverseRatio(float num) { materialData_->colorReverseRatio = num; }

		/// <summary>
		/// 色反転係数の取得
		/// </summary>
		/// <returns>色反転係数</returns>
		float GetColorReverseRatio() { return materialData_->colorReverseRatio; }

	};
}
#pragma once
#include <Base/View/ViewManagerBase.h>

namespace MyEngine {

	/// <summary>
	/// シェーダーリソースビューを管理する仕組みです。
	/// </summary>
	class SRVManager : public ViewManagerBase {

		///-------------------------------------------/// 
		/// 静的メンバ変数
		///-------------------------------------------///
	public:

		//最大SRV数
		static const uint32_t kMaxSRVCount_ = 512;

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// DSV用のデスクリプタヒープを初期化します。
		/// </summary>
		void Initialize(DirectXCommon* dxCommonPtr) override;

		/// <summary>
		/// 描画前処理
		/// </summary>
		void PreDraw() override;

		/// <summary>
		/// デスクリプタ番号の最大数を取得
		/// </summary>
		/// <returns>最大数のデスクリプタ番号</returns>
		uint32_t GetMaxDescriptorCount() override { return kMaxSRVCount_; }

	public:

		/// <summary>
		/// 2Dテクスチャ用のシェーダーリソースビュー(SRV)を生成します。
		/// </summary>
		void CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);

		/// <summary>
		/// 構造化バッファ用のシェーダーリソースビュー(SRV)を生成します。
		/// </summary>
		void CreateSRVForStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

		/// <summary>
		/// レンダーターゲット用のシェーダーリソースビュー(SRV)を生成します。
		/// </summary>
		void CreateRenderTargetSRV(uint32_t srvIndex, ID3D12Resource* pResource);

		/// <summary>
		/// 深度テクスチャ用のシェーダーリソースビュー(SRV)を生成します。
		/// </summary>
		void CreateDepthTextureSRV(uint32_t srvIndex, ID3D12Resource* pResource);

	public:

		/// <summary>
		/// デスクリプタテーブルの設定
		/// </summary>
		/// <param name="RootParameterIndex">パラメータ番号</param>
		/// <param name="srvIndex">SRV番号</param>
		void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);
	};
}
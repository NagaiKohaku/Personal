#pragma once
#include <Base/View/ViewManagerBase.h>

namespace MyEngine {

	/// <summary>
	/// DirectX12 の深度ステンシルビュー (DSV) を管理するクラスです。
	/// </summary>
	class DSVManager : public ViewManagerBase {

	public:

		DSVManager();

		~DSVManager();

		///-------------------------------------------/// 
		/// 静的メンバ変数
		///-------------------------------------------///
	public:

		//最大DSV数
		static const uint32_t kMaxDSVCount_ = 2;

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
		uint32_t GetMaxDescriptorCount() override { return kMaxDSVCount_; }

	public:

		/// <summary>
		/// 指定されたリソースに対してDSVを生成します。
		/// </summary>
		/// <param name="dsvIndex">割り当て済みのDSV番号</param>
		/// <param name="pResource">DSVを作成する対象のリソース</param>
		void CreateDepthStencilView(uint32_t dsvIndex, ID3D12Resource* pResource);

		/// <summary>
		/// 指定されたリソースに対して32ビット浮動小数点形式の深度ステンシルビューを生成します。
		/// </summary>
		/// <param name="dsvIndex">割り当て済みのDSV番号</param>
		/// <param name="pResource">DSVを作成する対象のリソース</param>
		void CreateDepthTexture(uint32_t dsvIndex, ID3D12Resource* pResource);
	};
}
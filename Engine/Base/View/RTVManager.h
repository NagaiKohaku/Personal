#pragma once
#include <Base/View/ViewManagerBase.h>

namespace MyEngine {

	/// <summary>
	/// 描画用レンダーターゲットビュー(RTV)の管理を行うクラスです。
	/// </summary>
	class RTVManager : public ViewManagerBase {

	public:

		RTVManager();

		~RTVManager();

		///-------------------------------------------/// 
		/// 静的メンバ変数
		///-------------------------------------------///
	public:

		//最大RTV数
		static const uint32_t kMaxRTVCount_ = 3;

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
		uint32_t GetMaxDescriptorCount() override { return kMaxRTVCount_; }

	public:

		/// <summary>
		/// 指定したリソースにRTVを作成します。
		/// </summary>
		void CreateRenderTargetView(uint32_t rtvIndex, ID3D12Resource* pResource);
	};
}
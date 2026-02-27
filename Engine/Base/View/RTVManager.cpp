#include "RTVManager.h"

#include <Base/DirectXCommon.h>

#include <cassert>

namespace MyEngine {

	RTVManager::RTVManager() = default;

	RTVManager::~RTVManager() = default;

	///=====================================================/// 
	/// RTV用のデスクリプタヒープを初期化
	///=====================================================///
	void RTVManager::Initialize(DirectXCommon* dxCommonPtr) {

		//DirectX基底のインスタンスを取得
		dxCommon_ = dxCommonPtr;

		//RTVデスクリプタヒープの初期化
		descriptorHeap_ = dxCommon_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kMaxRTVCount_, false);

		//RTVデスクリプタヒープのサイズを取得
		descriptorSize_ = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	///=====================================================/// 
	/// 描画前処理
	///=====================================================///
	void RTVManager::PreDraw() {
	}

	///=====================================================/// 
	/// 指定したリソースにRTVを作成
	///=====================================================///
	void RTVManager::CreateRenderTargetView(uint32_t rtvIndex, ID3D12Resource* pResource) {

		//RTVの情報
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

		//RTVの設定
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;      //出力結果をSRGBに変換して書き込む
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; //2dテクスチャとして書き込む

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = GetCPUDescriptorHandle(rtvIndex);

		//1つ目を作る
		dxCommon_->GetDevice()->CreateRenderTargetView(
			pResource,
			&rtvDesc,
			rtvHandle
		);
	}
}
#include "RTVManager.h"

#include "Base/DirectXCommon.h"

#include "cassert"

RTVManager* RTVManager::GetInstance() {
	static RTVManager instance;
	return &instance;
}

void RTVManager::Initialize() {

	//DirectX基底のインスタンスを取得
	directXCommon = DirectXCommon::GetInstance();

	//RTVデスクリプタヒープの初期化
	rtvDescriptorHeap_ = directXCommon->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kMaxRTVCount_, false);

	//RTVデスクリプタヒープのサイズを取得
	rtvDescriptorSize_ = directXCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

void RTVManager::PreDraw() {
}

uint32_t RTVManager::Allocate() {

	//RTV番号が最大数を越えていないかの確認
	assert(kMaxRTVCount_ > useIndex_);

	//現在のRTV番号を返す
	int index = useIndex_;

	//1つずらした番号を確保する
	useIndex_++;

	return index;
}

bool RTVManager::AllocateCheck() {

	//RTV番号が最大数を越えていないかの確認
	if (kMaxRTVCount_ > useIndex_) {
		return true;
	}

	return false;
}

void RTVManager::CreateRenderTargetView(uint32_t rtvIndex, ID3D12Resource* pResource) {

	//RTVの情報
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//RTVの設定
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;      //出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; //2dテクスチャとして書き込む

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = GetCPUDescriptorHandle(rtvIndex);

	//1つ目を作る
	directXCommon->GetDevice()->CreateRenderTargetView(
		pResource,
		&rtvDesc,
		rtvHandle
	);
}

D3D12_CPU_DESCRIPTOR_HANDLE RTVManager::GetCPUDescriptorHandle(uint32_t index) {

	//デスクリプタの最初のメモリを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();

	//メモリを番号分進ませる
	handleCPU.ptr += (rtvDescriptorSize_ * index);

	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE RTVManager::GetGPUDescriptorHandle(uint32_t index) {

	//デスクリプタの最初のメモリを取得
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = rtvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();

	//メモリを番号分進ませる
	handleGPU.ptr += (rtvDescriptorSize_ * index);

	return handleGPU;
}

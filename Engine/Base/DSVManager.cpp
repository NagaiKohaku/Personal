#include "DSVManager.h"

#include "Base/DirectXCommon.h"

#include "cassert"

DSVManager* DSVManager::GetInstance() {
	static DSVManager instance;
	return &instance;
}

void DSVManager::Initialize() {

	//DirectX基底のインスタンスを取得
	directXCommon = DirectXCommon::GetInstance();

	//DSVデスクリプタヒープの初期化
	dsvDescriptorHeap_ = directXCommon->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, kMaxDSVCount_, false);

	//DSVデスクリプタヒープのサイズを取得
	dsvDescriptorSize_ = directXCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void DSVManager::PreDraw() {
}

uint32_t DSVManager::Allocate() {

	//DSV番号が最大数を越えていないかの確認
	assert(kMaxDSVCount_ > useIndex_);

	//現在のDSV番号を返す
	int index = useIndex_;

	//1つずらした番号を確保する
	useIndex_++;

	return index;
}

bool DSVManager::AllocateCheck() {

	//DSV番号が最大数を越えていないかの確認
	if (kMaxDSVCount_ > useIndex_) {
		return true;
	}

	return false;
}

void DSVManager::CreateDepthStencilView(uint32_t dsvIndex, ID3D12Resource* pResource) {

	//DSVの情報
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

	//DSVの設定
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;        //Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; //2DTexture

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = GetCPUDescriptorHandle(dsvIndex);

	//DSVHeapの先頭にDSVを作る
	directXCommon->GetDevice()->CreateDepthStencilView(
		pResource,
		&dsvDesc,
		dsvHandle
	);

}

D3D12_CPU_DESCRIPTOR_HANDLE DSVManager::GetCPUDescriptorHandle(uint32_t index) {

	//デスクリプタの最初のメモリを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();

	//メモリを番号分進ませる
	handleCPU.ptr += (dsvDescriptorSize_ * index);

	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DSVManager::GetGPUDescriptorHandle(uint32_t index) {

	//デスクリプタの最初のメモリを取得
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = dsvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();

	//メモリを番号分進ませる
	handleGPU.ptr += (dsvDescriptorSize_ * index);

	return handleGPU;
}

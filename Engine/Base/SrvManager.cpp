#include "SrvManager.h"

#include "Base/DirectXCommon.h"

#include "cassert"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
SrvManager* SrvManager::GetInstance() {
	static SrvManager instance;
	return &instance;
}

///=====================================================/// 
/// 初期化処理
///=====================================================///
void SrvManager::Initialize() {

	//DirextX基底のインスタンスを取得
	directXCommon = DirectXCommon::GetInstance();

	//srvデスクリプタヒープの初期化
	srvDescriptorHeap_ = directXCommon->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount_, true);

	//srvデスクリプタヒープのサイズを取得
	srvDescriptorSize_ = directXCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

///=====================================================/// 
/// 描画前処理
///=====================================================///
void SrvManager::PreDraw() {

	//描画前のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap_.Get() };

	//コマンドリストにsrvデスクリプタヒープを設定する
	directXCommon->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

///=====================================================/// 
/// メモリ確保
///=====================================================///
uint32_t SrvManager::Allocate() {

	//SRV番号が最大数を越えていないかの確認
	assert(kMaxSRVCount_ > useIndex_);

	//現在のsrv番号を返す
	int index = useIndex_;

	//1つずらした番号を確保する
	useIndex_++;

	return index;
}

///=====================================================/// 
/// メモリ確保可能チェック
///=====================================================///
bool SrvManager::AllocateCheck() {

	//SRV番号が最大数を越えていないかの確認
	if (kMaxSRVCount_ > useIndex_) {
		return true;
	}

	return false;
}

///=====================================================/// 
/// SRV生成(テクスチャ用)
///=====================================================///
void SrvManager::CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels) {

	//SRVの情報
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	//SRVの設定
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(MipLevels);

	//SRVの生成
	directXCommon->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

///=====================================================/// 
///SRV生成(Structured Buffer用)
///=====================================================///
void SrvManager::CreateSRVForStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride) {

	//SRVの情報
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	//SRVの設定
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = structureByteStride;

	//SRVの生成
	directXCommon->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

///=====================================================/// 
/// CPUデスクリプターのゲッター
///=====================================================///
D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index) {

	//デスクリプタの最初のメモリを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();

	//メモリを番号分進ませる
	handleCPU.ptr += (srvDescriptorSize_ * index);

	return handleCPU;
}

///=====================================================/// 
/// GPUデスクリプターのゲッター
///=====================================================///
D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index) {

	//デスクリプタの最初のメモリを取得
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();

	//メモリを番号分進ませる
	handleGPU.ptr += (srvDescriptorSize_ * index);

	return handleGPU;
}

///=====================================================/// 
/// デスクリプタテーブルのセッター
///=====================================================///
void SrvManager::SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex) {

	directXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(RootParameterIndex, GetGPUDescriptorHandle(srvIndex));
}
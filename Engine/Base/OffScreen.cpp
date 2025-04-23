#include "OffScreen.h"

#include "Base/DirectXCommon.h"
#include "Base/SrvManager.h"

void OffScreen::Initialize() {

	//DirectX基底のインスタンスを取得
	dxCommon_ = DirectXCommon::GetInstance();
	//SRVマネージャーのインスタンスを取得
	srvManager_ = SrvManager::GetInstance();

	CreateRenderTargetView();
	CreateShaderResourceView();

	//ルートシグネチャの生成
	CreateRootSignature();
	//パイプラインの生成
	CreatePipeline();
}

void OffScreen::PreDraw() {

	/// === 現在のバッファをコマンドが詰めるようにする === ///

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};

	//バリアはTransitionタイプ
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	//フラグはNone
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = offScreenResrouce_.Get();

	//現在のバッファをコマンドの命令待機状態に設定
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//バッファの次の命令を描画状態に設定
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//TransitionBarrierを張る
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);

	/// === 画面をクリアする === ///

	//DSVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	//DSVを取得
	dsvHandle = dxCommon_->GetCPUDescriptorHandle(dsvDescriptorHeap_, descriptorSizeDSV_, 0);

	//RTVとDSVに描画先を設定する
	dxCommon_->GetCommandList()->OMSetRenderTargets(1, &offScreenRTVHandle_, false, &dsvHandle);

	//指定した色で画面全体をクリアする
	float clearColor[] = {
		offScreenClearColor_.x,
		offScreenClearColor_.y,
		offScreenClearColor_.z,
		offScreenClearColor_.w,
	};

	dxCommon_->GetCommandList()->ClearRenderTargetView(offScreenRTVHandle_, clearColor, 0, nullptr);

	//指定した深度で画面全体をクリアする
	dxCommon_->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	/// === 描画範囲の設定 === ///

	//Viewportを設定
	dxCommon_->GetCommandList()->RSSetViewports(1, &viewport_);

	//Scissor設定
	dxCommon_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);

}

void OffScreen::PostDraw() {

	/// === 現在のバッファを描画ができるようにする=== ///

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};

	//バリアのタイプはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	//フラグはNone
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = offScreenResrouce_.Get();

	//現在のバッファを描画状態に設定
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//バッファの次の命令をコマンドの命令待機状態に設定
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//TransitionBarrierを張る
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);

	dxCommon_->GetCommandList()->SetGraphicsRootSignature(offScreenRootSignature_.Get());

	dxCommon_->GetCommandList()->SetPipelineState(offScreenGraphicsPipelineState_[0].Get());

	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(0, offScreenSRVHandle_);
}

void OffScreen::CreateRootSignature() {
}

void OffScreen::CreatePipeline() {
}

void OffScreen::CreateRenderTargetView() {

}

void OffScreen::CreateShaderResourceView() {

	srvIndex_ = srvManager_->Allocate();

	srvManager_->CreateRenderTargetSRV(srvIndex_, offScreenResrouce_.Get());
}

Microsoft::WRL::ComPtr<ID3D12Resource> OffScreen::CreateRenderTexture(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor) {
	return Microsoft::WRL::ComPtr<ID3D12Resource>();
}

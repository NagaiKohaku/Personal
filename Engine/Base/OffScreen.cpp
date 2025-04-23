#include "OffScreen.h"

#include "Base/DirectXCommon.h"
#include "Base/SrvManager.h"

void OffScreen::Initialize() {

	//DirectX���̃C���X�^���X���擾
	dxCommon_ = DirectXCommon::GetInstance();
	//SRV�}�l�[�W���[�̃C���X�^���X���擾
	srvManager_ = SrvManager::GetInstance();

	CreateRenderTargetView();
	CreateShaderResourceView();

	//���[�g�V�O�l�`���̐���
	CreateRootSignature();
	//�p�C�v���C���̐���
	CreatePipeline();
}

void OffScreen::PreDraw() {

	/// === ���݂̃o�b�t�@���R�}���h���l�߂�悤�ɂ��� === ///

	//TransitionBarrier�̐ݒ�
	D3D12_RESOURCE_BARRIER barrier{};

	//�o���A��Transition�^�C�v
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	//�t���O��None
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	//�o���A�𒣂�Ώۂ̃��\�[�X�B���݂̃o�b�N�o�b�t�@�ɑ΂��čs��
	barrier.Transition.pResource = offScreenResrouce_.Get();

	//���݂̃o�b�t�@���R�}���h�̖��ߑҋ@��Ԃɐݒ�
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//�o�b�t�@�̎��̖��߂�`���Ԃɐݒ�
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//TransitionBarrier�𒣂�
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);

	/// === ��ʂ��N���A���� === ///

	//DSV�n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	//DSV���擾
	dsvHandle = dxCommon_->GetCPUDescriptorHandle(dsvDescriptorHeap_, descriptorSizeDSV_, 0);

	//RTV��DSV�ɕ`����ݒ肷��
	dxCommon_->GetCommandList()->OMSetRenderTargets(1, &offScreenRTVHandle_, false, &dsvHandle);

	//�w�肵���F�ŉ�ʑS�̂��N���A����
	float clearColor[] = {
		offScreenClearColor_.x,
		offScreenClearColor_.y,
		offScreenClearColor_.z,
		offScreenClearColor_.w,
	};

	dxCommon_->GetCommandList()->ClearRenderTargetView(offScreenRTVHandle_, clearColor, 0, nullptr);

	//�w�肵���[�x�ŉ�ʑS�̂��N���A����
	dxCommon_->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	/// === �`��͈͂̐ݒ� === ///

	//Viewport��ݒ�
	dxCommon_->GetCommandList()->RSSetViewports(1, &viewport_);

	//Scissor�ݒ�
	dxCommon_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);

}

void OffScreen::PostDraw() {

	/// === ���݂̃o�b�t�@��`�悪�ł���悤�ɂ���=== ///

	//TransitionBarrier�̐ݒ�
	D3D12_RESOURCE_BARRIER barrier{};

	//�o���A�̃^�C�v��Transition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	//�t���O��None
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	//�o���A�𒣂�Ώۂ̃��\�[�X�B���݂̃o�b�N�o�b�t�@�ɑ΂��čs��
	barrier.Transition.pResource = offScreenResrouce_.Get();

	//���݂̃o�b�t�@��`���Ԃɐݒ�
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//�o�b�t�@�̎��̖��߂��R�}���h�̖��ߑҋ@��Ԃɐݒ�
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//TransitionBarrier�𒣂�
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

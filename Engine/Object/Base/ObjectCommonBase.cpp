#include "ObjectCommonBase.h"

#include <Base/DirectXCommon.h>
#include <Other/Log.h>

#include <cassert>
#include <numbers>

using namespace MyEngine;

ObjectCommonBase::ObjectCommonBase() = default;

ObjectCommonBase::~ObjectCommonBase() = default;

void ObjectCommonBase::Initialize(DirectXCommon* dxCommonPtr) {

	//DirectX基底のインスタンスを取得
	dxCommon_ = dxCommonPtr;

	//グラフィックパイプラインの生成
	CreateGraphicsPipeline();
}

void ObjectCommonBase::CommonDrawSetting() {

	//RootSignatureを設定
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());

	//PSOを設定
	dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState_.Get());

	//メッシュトポロジーを設定
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

D3D12_DEPTH_STENCIL_DESC MyEngine::ObjectCommonBase::CreateDepthStencil() {

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;

	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	return depthStencilDesc;
}

D3D12_RASTERIZER_DESC MyEngine::ObjectCommonBase::CreateRasterizer() {

	D3D12_RASTERIZER_DESC rasterizerDesc{};

	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;

	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	return rasterizerDesc;

}

D3D12_BLEND_DESC MyEngine::ObjectCommonBase::CreateBlend() {

	D3D12_BLEND_DESC blendDesc{};

	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	return blendDesc;
}

void ObjectCommonBase::CreateGraphicsPipeline() {

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
	Microsoft::WRL::ComPtr<IDxcBlob> vsBlob;
	Microsoft::WRL::ComPtr<IDxcBlob> psBlob;

	CreateRootSignature();

	CreateInputElement(inputElements);

	CreateShader(vsBlob, psBlob);

	HRESULT hr;

	/// === PSOを生成する === ///

	//PSOの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	//RootSignature
	graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();

	//InputLayout
	graphicsPipelineStateDesc.InputLayout = { inputElements.data(), static_cast<UINT>(inputElements.size()) };

	//VertexShader
	graphicsPipelineStateDesc.VS = { vsBlob->GetBufferPointer(), vsBlob->GetBufferSize() };

	//PixelShader
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(), psBlob->GetBufferSize() };

	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = CreateDepthStencil();
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//RasterizerState
	graphicsPipelineStateDesc.RasterizerState = CreateRasterizer();

	//BlendState
	graphicsPipelineStateDesc.BlendState = CreateBlend();

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;

	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//どのように画面に色を打ち込むかの設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;

	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//PSOを生成
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(
		&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState_)
	);

	//正常に生成できているかの確認
	assert(SUCCEEDED(hr));
}

void MyEngine::ObjectCommonBase::CreateDescriptorRootSignature(
	D3D12_ROOT_PARAMETER* rootParameters,
	UINT numRootParameters,
	D3D12_STATIC_SAMPLER_DESC* staticSamplers,
	UINT numStaticSamplers
) {

	HRESULT hr;

	//ルートシグネチャ
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

	//シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	descriptionRootSignature.pParameters = rootParameters;               //ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = numRootParameters;   //配列の長さ

	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = numStaticSamplers;

	hr = D3D12SerializeRootSignature(
		&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&signatureBlob,
		&errorBlob
	);

	//エラーが出たら止める
	if (FAILED(hr)) {

		OutPutLog(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));

		assert(false);
	}

	hr = dxCommon_->GetDevice()->CreateRootSignature(
		0,
		signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_)
	);

	assert(SUCCEEDED(hr));
}
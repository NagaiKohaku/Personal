#include "Object2DCommon.h"

#include <Base/DirectXCommon.h>

#include <cassert>

using namespace MyEngine;

Object2DCommon::Object2DCommon() = default;

Object2DCommon::~Object2DCommon() = default;

void Object2DCommon::Initialize(DirectXCommon* dxCommonPtr) {

	ObjectCommonBase::Initialize(dxCommonPtr);

	/// === 頂点リソースの作成 === ///

	//リソースを作成
	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * 4);

	//リソースの先頭のアドレスを取得する
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;

	//1頂点当たりのサイズを設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//書き込むためのアドレスを取得する
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//左下
	vertexData_[0].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData_[0].texcoord = { 0.0f,0.0f };

	//左上
	vertexData_[1].position = { 0.0f,1.0f,0.0f,1.0f };
	vertexData_[1].texcoord = { 0.0f,1.0f };

	//右下
	vertexData_[2].position = { 1.0f,0.0f,0.0f,1.0f };
	vertexData_[2].texcoord = { 1.0f,0.0f };

	//右上
	vertexData_[3].position = { 1.0f,1.0f,0.0f,1.0f };
	vertexData_[3].texcoord = { 1.0f,1.0f };

	/// === 頂点インデックスリソースの作成 === ///

	//リソースを作成
	IndexResource_ = dxCommon_->CreateBufferResource(sizeof(uint32_t) * 6);

	//リソースの先頭のアドレスを取得する
	indexBufferView_.BufferLocation = IndexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;

	//フォーマットを設定
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//書き込むためのアドレスを取得する
	IndexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	//頂点インデックスのデータを書き込む
	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;

}

void Object2DCommon::CommonDrawSetting() {

	ObjectCommonBase::CommonDrawSetting();
}

///=====================================================/// 
/// 2Dオブジェクト描画用のルートシグネチャを作成
///=====================================================///
void Object2DCommon::CreateRootSignature() {

	/// === DescriptorRangeの設定 === ///

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};

	descriptorRange[0].BaseShaderRegister = 0; //0番目から使用
	descriptorRange[0].NumDescriptors = 1; //使用するTextureは1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //Offsetを自動計算

	/// === RootParameterの設定 === ///

	D3D12_ROOT_PARAMETER rootParameters[3] = {};

	//マテリアル
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; //CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderを使う
	rootParameters[0].Descriptor.ShaderRegister = 0; //0番目のレジスタを使う

	//WVP
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //VertexShaderを使う
	rootParameters[1].Descriptor.ShaderRegister = 0; //0番目のレジスタを使う

	//テクスチャ
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; //DesctiptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderを使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; //Resrouceの範囲を設定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); //Resourceの数を設定

	/// === Samplerの設定 === ///

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};

	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; //バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; //ありったけのMinMapを使う
	staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う

	ObjectCommonBase::CreateDescriptorRootSignature(
		rootParameters,
		_countof(rootParameters),
		staticSamplers,
		_countof(staticSamplers)
	);
}

void Object2DCommon::CreateInputElement(
	std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElements
) {

	//InputLayOutを設定する
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};

	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	for (int i = 0; i < _countof(inputElementDescs); i++) {
		inputElements.push_back(inputElementDescs[i]);
	}
}

void Object2DCommon::CreateShader(
	Microsoft::WRL::ComPtr<IDxcBlob>& vs,
	Microsoft::WRL::ComPtr<IDxcBlob>& ps
) {

	//VertexShaderをコンパイルする
	vs = dxCommon_->CompileShader(
		L"Resource/Shader/Object2D.VS.hlsl",
		L"vs_6_0"
	);

	assert(vs != nullptr);

	//PixelShaderをコンパイルする
	ps = dxCommon_->CompileShader(
		L"Resource/Shader/Object2D.PS.hlsl",
		L"ps_6_0"
	);

	assert(ps != nullptr);
}
#include "Object3DCommon.h"

#include <Base/DirectXCommon.h>
#include <3d/Camera/Camera.h>
#include <2d/Sprite/TextureManager.h>

#include <cassert>

using namespace MyEngine;

Object3DCommon::Object3DCommon() = default;

Object3DCommon::~Object3DCommon() = default;

void Object3DCommon::Initialize(DirectXCommon* dxCommonPtr) {

	ObjectCommonBase::Initialize(dxCommonPtr);

	//平行光源ライトの生成
	directionalLight_ = std::make_unique<DirectionalLight>();

	//平行光源ライトの初期化
	directionalLight_->Initialize(dxCommon_);

	directionalLight_->SetIntensity(1.0f);

	//点光源ライトの生成
	pointLight_ = std::make_unique<PointLight>();

	//点光源ライトの初期化
	pointLight_->Initialize(dxCommon_);

	pointLight_->SetIntensity(0.0f);

	//スポットライトの生成
	spotLight_ = std::make_unique<SpotLight>();

	//スポットライトの初期化
	spotLight_->Initialize(dxCommon_);

	spotLight_->SetIntensity(0.0f);

	//カメラ情報のバッファリソースの生成
	cameraForGpuResource = dxCommon_->CreateBufferResource(sizeof(CameraForGPU));

	//リソースにカメラ情報を記録
	cameraForGpuResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGpuData));

	//カメラ情報の設定
	cameraForGpuData->worldPosition = Vector3(0.0f, 0.0f, 0.0f);

	textureCubeFilePath_ = "Resource/Texture/CubeTexture/Transparent.png";

	TextureManager::GetInstance()->LoadCubeTexture(textureCubeFilePath_);
}

void Object3DCommon::CommonDrawSetting() {

	//カメラ位置を取得
	cameraForGpuData->worldPosition = defaultCamera_->GetWorldTransform().GetWorldTranslate();

	//平行光源ライトの更新
	directionalLight_->Update();

	//点光源ライトの更新
	pointLight_->Update();

	//スポットライトの更新
	spotLight_->Update();

	ObjectCommonBase::CommonDrawSetting();

	//カメラ情報の設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, cameraForGpuResource.Get()->GetGPUVirtualAddress());

	//平行光源ライトのデータをGPUに送信
	directionalLight_->SendDataForGPU();

	//点光源ライトのデータをGPUに送信
	pointLight_->SendDataForGPU();

	//スポットライトのデータをGPUに送信
	spotLight_->SendDataForGPU();

	if (textureCubeFilePath_ != "") {

		//テクスチャデータの設定
		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(7, TextureManager::GetInstance()->GetSrvHandleGPU(textureCubeFilePath_));
	}
}

///=====================================================/// 
/// 2Dオブジェクト描画用のルートシグネチャを作成
///=====================================================///
void Object3DCommon::CreateRootSignature() {

	/// === DescriptorRangeの設定 === ///

	D3D12_DESCRIPTOR_RANGE textureRange[1] = {};

	textureRange[0].BaseShaderRegister = 0; //0から始まる
	textureRange[0].NumDescriptors = 1; //数は1つ
	textureRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
	textureRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE textureCubeRange[1] = {};

	textureCubeRange[0].BaseShaderRegister = 1; //1から始まる
	textureCubeRange[0].NumDescriptors = 1; //数は1つ
	textureCubeRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
	textureCubeRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //Offsetを自動計算

	/// === RootParameterの設定 === ///

		//RootParameterを作成
	D3D12_ROOT_PARAMETER rootParameters[8] = {};

	//マテリアル
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                   //CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                //PixelShaderを使う
	rootParameters[0].Descriptor.ShaderRegister = 0;                                   //レジスタ番号0とバインド

	//WVP
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                   //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;               //VertexShaderを使う
	rootParameters[1].Descriptor.ShaderRegister = 0;                                   //レジスタ番号0とバインド

	//テクスチャ
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      //DesctiptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                //PixelShaderを使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = textureRange;             //Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(textureRange); //Tableで利用する数
	rootParameters[2].Descriptor.RegisterSpace = 0;                                    //レジスタ番号0を使う

	//カメラ
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                   //CBVを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                //PixelShaderを使う
	rootParameters[3].Descriptor.ShaderRegister = 1;                                   //レジスタ番号1を使う

	//平行光源ライト
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                   //CBVを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                //PixelShaderを使う
	rootParameters[4].Descriptor.ShaderRegister = 2;                                   //レジスタ番号2を使う

	//点光源ライト
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                   //CBVを使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                //PixelShaderを使う
	rootParameters[5].Descriptor.ShaderRegister = 3;                                   //レジスタ番号3を使う

	//スポットライト
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;                   //CBVを使う
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                //PixelShaderを使う
	rootParameters[6].Descriptor.ShaderRegister = 4;                                   //レジスタ番号4を使う

	//スカイボックステクスチャ
	rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      //DesctiptorTableを使う
	rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                //PixelShaderを使う
	rootParameters[7].DescriptorTable.pDescriptorRanges = textureCubeRange;             //Tableの中身の配列を指定
	rootParameters[7].DescriptorTable.NumDescriptorRanges = _countof(textureCubeRange); //Tableで利用する数
	rootParameters[7].Descriptor.RegisterSpace = 1;                                    //レジスタ番号1を使う

	/// === Samplerの設定 === ///

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};

	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         //バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       //0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;     //比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       //ありったけのMinMapを使う
	staticSamplers[0].ShaderRegister = 0;                               //レジスタ番号0を使う

	ObjectCommonBase::CreateDescriptorRootSignature(
		rootParameters,
		_countof(rootParameters),
		staticSamplers,
		_countof(staticSamplers)
	);
}

void Object3DCommon::CreateInputElement(
	std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElements
) {

	//InputLayOutを設定する
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};

	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	for (int i = 0; i < _countof(inputElementDescs); i++) {
		inputElements.push_back(inputElementDescs[i]);
	}
}

void Object3DCommon::CreateShader(
	Microsoft::WRL::ComPtr<IDxcBlob>& vs,
	Microsoft::WRL::ComPtr<IDxcBlob>& ps
) {

	//VertexShaderをコンパイルする
	vs = dxCommon_->CompileShader(
		L"Resource/Shader/Object3D.VS.hlsl",
		L"vs_6_0"
	);

	assert(vs != nullptr);

	//PixelShaderをコンパイルする
	ps = dxCommon_->CompileShader(
		L"Resource/Shader/Object3D.PS.hlsl",
		L"ps_6_0"
	);

	assert(ps != nullptr);
}
#include "Object3DCommon.h"

#include "Base/DirectXCommon.h"
#include "3d/Camera/Camera.h"

#include "Other/Log.h"

#include "cassert"

#include "numbers"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
Object3DCommon* Object3DCommon::GetInstance() {
	static Object3DCommon instance;
	return &instance;
}

///=====================================================/// 
/// 初期化処理
///=====================================================///
void Object3DCommon::Initialize() {

	//DirectX基底のインスタンスを取得
	dxCommon_ = DirectXCommon::GetInstance();

	//グラフィックパイプラインの生成
	CreateGraphicsPipeline();

	//平行光源ライトの生成
	directionalLight_ = std::make_unique<DirectionalLight>();

	//平行光源ライトの初期化
	directionalLight_->Initialize();

	directionalLight_->SetIntensity(1.0f);

	//点光源ライトの生成
	pointLight_ = std::make_unique<PointLight>();

	//点光源ライトの初期化
	pointLight_->Initialize();

	pointLight_->SetIntensity(0.0f);

	//スポットライトの生成
	spotLight_ = std::make_unique<SpotLight>();

	//スポットライトの初期化
	spotLight_->Initialize();

	spotLight_->SetIntensity(0.0f);

	//カメラ情報のバッファリソースの生成
	cameraForGpuResource = dxCommon_->CreateBufferResource(sizeof(CameraForGPU));

	//リソースにカメラ情報を記録
	cameraForGpuResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGpuData));

	//カメラ情報の設定
	cameraForGpuData->worldPosition = Vector3(0.0f, 0.0f, 0.0f);

	//ブレンドモードをNormalに設定
	blendMode_ = Normal;
}

///=====================================================/// 
/// 更新処理
///=====================================================///
void Object3DCommon::Update() {

	//カメラ位置を取得
	cameraForGpuData->worldPosition = defaultCamera_->GetWorldTransform().GetWorldTranslate();

	//平行光源ライトのImGuiを表示
	directionalLight_->DisplayImGui();

	//平行光源ライトの更新
	directionalLight_->Update();

	//点光源ライトのImGuiを表示
	pointLight_->DisplayImGui();

	//点光源ライトの更新
	pointLight_->Update();

	//スポットライトのImGuiを表示
	spotLight_->DisplayImGui();

	//スポットライトの更新
	spotLight_->Update();
}

///=====================================================/// 
/// 描画前処理
///=====================================================///
void Object3DCommon::CommonDrawSetting() {

	//ルートシグネチャを設定
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());

	//グラフィックパイプラインを設定
	dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState_[blendMode_].Get());

	//プリミティブトポロジーを設定
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//カメラ情報の設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, cameraForGpuResource.Get()->GetGPUVirtualAddress());

	//平行光源ライトのデータをGPUに送信
	directionalLight_->SendDataForGPU();

	//点光源ライトのデータをGPUに送信
	pointLight_->SendDataForGPU();

	//スポットライトのデータをGPUに送信
	spotLight_->SendDataForGPU();
}

///=====================================================/// 
/// ルートシグネチャの生成
///=====================================================///
void Object3DCommon::CreateRootSignature() {

	HRESULT hr;

	//ルートシグネチャ
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; //0から始まる
	descriptorRange[0].NumDescriptors = 1; //数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //Offsetを自動計算

	//RootParameterを作成
	D3D12_ROOT_PARAMETER rootParameters[7] = {};

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
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;             //Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); //Tableで利用する数

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

	descriptionRootSignature.pParameters = rootParameters;               //ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);   //配列の長さ

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         //バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       //0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;     //比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       //ありったけのMinMapを使う
	staticSamplers[0].ShaderRegister = 0;                               //レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	//シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	//エラーが出たら止める
	if (FAILED(hr)) {

		OutPutLog(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));

		assert(false);
	}

	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));

	assert(SUCCEEDED(hr));
}

///=====================================================/// 
/// グラフィックパイプラインの生成
///=====================================================///
void Object3DCommon::CreateGraphicsPipeline() {

	HRESULT hr;

	/// === RootSignatureを設定する === ///

	CreateRootSignature();

	/// === InputLayoutを設定する === ///

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

	//InputLayoutを確定する
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	/// === BlendStateを設定する === ///

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};

	//Normalのブレンドモードで設定
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	/// === RasterizerStateを設定する === ///

	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;

	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	/// === Shaderのコンパイル === ///

	//VertexShaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob =
		dxCommon_->CompileShader(
			L"Resource/Shader/Object3D.VS.hlsl",
			L"vs_6_0"
		);

	assert(vertexShaderBlob != nullptr);

	//PixelShaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob =
		dxCommon_->CompileShader(
			L"Resource/Shader/Object3D.PS.hlsl",
			L"ps_6_0"
		);

	assert(pixelShaderBlob != nullptr);

	/// === DepthStencilStateの設定 === ///

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	/// === PSOを生成する === ///

	//PSOの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	//PSO
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;

	//RootSignature
	graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();

	//InputLayout
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;

	//VertexShader
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() };

	//PixelShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
	pixelShaderBlob->GetBufferSize() };

	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//BlendState
	graphicsPipelineStateDesc.BlendState = blendDesc;

	//RasterizerState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

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
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));

	//正常に生成できているかの確認
	assert(SUCCEEDED(hr));

	//配列に追加
	graphicsPipelineState_.push_back(graphicsPipelineState);

	/// === ブレンドモードAddのPSOを生成 === ///

	graphicsPipelineStateDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	graphicsPipelineStateDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	graphicsPipelineStateDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	//PSOを生成
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));

	//正常に生成できているかの確認
	assert(SUCCEEDED(hr));

	//配列に追加
	graphicsPipelineState_.push_back(graphicsPipelineState);

	/// === ブレンドモードSubtractのPSOを生成 === ///

	graphicsPipelineStateDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	graphicsPipelineStateDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	graphicsPipelineStateDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	//PSOを生成
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));

	//正常に生成できているかの確認
	assert(SUCCEEDED(hr));

	//配列に追加
	graphicsPipelineState_.push_back(graphicsPipelineState);

	/// === ブレンドモードMultilyのPSOを生成 === ///

	graphicsPipelineStateDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
	graphicsPipelineStateDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	graphicsPipelineStateDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;

	//PSOを生成
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));

	//正常に生成できているかの確認
	assert(SUCCEEDED(hr));

	//配列に追加
	graphicsPipelineState_.push_back(graphicsPipelineState);

	/// === ブレンドモードScreenのPSOを生成 === ///

	graphicsPipelineStateDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	graphicsPipelineStateDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	graphicsPipelineStateDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	//PSOを生成
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));

	//正常に生成できているかの確認
	assert(SUCCEEDED(hr));

	//配列に追加
	graphicsPipelineState_.push_back(graphicsPipelineState);

}
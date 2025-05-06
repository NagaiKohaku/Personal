#include "OffScreen.h"

#include "Base/WinApp.h"
#include "Base/DirectXCommon.h"
#include "Base/RTVManager.h"
#include "Base/DSVManager.h"
#include "Base/SrvManager.h"
#include "Other/Log.h"

#include "cassert"

OffScreen* OffScreen::GetInstance() {
	static OffScreen instance;
	return &instance;
}

void OffScreen::Initialize() {

	//DirectX基底のインスタンスを取得
	dxCommon_ = DirectXCommon::GetInstance();
	//RTVマネージャーのインスタンスを取得
	rtvManager_ = RTVManager::GetInstance();
	//DSVマネージャーのインスタンスを取得
	dsvManager_ = DSVManager::GetInstance();
	//SRVマネージャーのインスタンスを取得
	srvManager_ = SrvManager::GetInstance();

	renderTextureResrouce_ = CreateRenderTexture(
		dxCommon_->GetDevice(),
		WinApp::kClientWidth,
		WinApp::kClientHeight,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		offScreenClearColor_
	);

	CreateRenderTargetView();

	CreateDepthStencilView();

	CreateShaderResourceView();

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
	barrier.Transition.pResource = renderTextureResrouce_.Get();

	//現在のバッファをコマンドの命令待機状態に設定
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//バッファの次の命令を描画状態に設定
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//TransitionBarrierを張る
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);

	/// === 画面をクリアする === ///

	//RTVとDSVに描画先を設定する
	dxCommon_->GetCommandList()->OMSetRenderTargets(1, &offScreenRTVHandle_, false, &offScreenDSVHandle_);

	//指定した色で画面全体をクリアする
	float clearColor[] = {
		offScreenClearColor_.x,
		offScreenClearColor_.y,
		offScreenClearColor_.z,
		offScreenClearColor_.w,
	};

	dxCommon_->GetCommandList()->ClearRenderTargetView(offScreenRTVHandle_, clearColor, 0, nullptr);

	//指定した深度で画面全体をクリアする
	dxCommon_->GetCommandList()->ClearDepthStencilView(offScreenDSVHandle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	/// === 描画範囲の設定 === ///

	D3D12_VIEWPORT viewport = dxCommon_->GetViewport(); //Viewportを取得

	D3D12_RECT scissorRect = dxCommon_->GetScissorRect(); //Scissorを取得

	//Viewportを設定
	dxCommon_->GetCommandList()->RSSetViewports(1, &viewport);

	//Scissor設定
	dxCommon_->GetCommandList()->RSSetScissorRects(1, &scissorRect);

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
	barrier.Transition.pResource = renderTextureResrouce_.Get();

	//現在のバッファを描画状態に設定
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//バッファの次の命令をコマンドの命令待機状態に設定
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//TransitionBarrierを張る
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);

}

void OffScreen::DrawToSwapChain() {

	dxCommon_->GetCommandList()->SetGraphicsRootSignature(offScreenRootSignature_.Get());

	dxCommon_->GetCommandList()->SetPipelineState(offScreenGraphicsPipelineState_[0].Get());

	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(0, srvManager_->GetGPUDescriptorHandle(srvIndex_));

	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void OffScreen::CreateRootSignature() {

	HRESULT hr;

	//RootSignatureを作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; //0から始まる
	descriptorRange[0].NumDescriptors = 1; //数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //Offsetを自動計算

	//RootParameterを作成
	D3D12_ROOT_PARAMETER rootParameters[1] = {};

	//テクスチャ
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      //DesctiptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                //PixelShaderを使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;             //Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); //Tableで利用する数

	descriptionRootSignature.pParameters = rootParameters;               //ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);   //配列の長さ

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; //バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; //ありったけのMinMapを使う
	staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
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
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&offScreenRootSignature_));

	assert(SUCCEEDED(hr));
}

void OffScreen::CreatePipeline() {

	HRESULT hr;

	/// === RootSignatureを設定する === ///

	CreateRootSignature();

	/// === InputLayoutを設定する === ///

	//InputLayoutを確定する
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;

	/// === BlendStateを設定する === ///

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};

	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;

	/// === RasterizerStateを設定する === ///

	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;

	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	/// === Shaderのコンパイル === ///

	//VertexShaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob =
		dxCommon_->CompileShader(
			L"Resource/Shader/CopyImage.VS.hlsl",
			L"vs_6_0"
		);

	assert(vertexShaderBlob != nullptr);

	//PixelShaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob =
		dxCommon_->CompileShader(
			L"Resource/Shader/CopyImage.PS.hlsl",
			L"ps_6_0"
		);

	assert(pixelShaderBlob != nullptr);

	/// === DepthStencilStateの生成 === ///

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = false;

	/// === PSOを生成する === ///

	//PSOを生成する
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	//PSO
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;

	//RootSignature
	graphicsPipelineStateDesc.pRootSignature = offScreenRootSignature_.Get();

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
	offScreenGraphicsPipelineState_.push_back(graphicsPipelineState);

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
	offScreenGraphicsPipelineState_.push_back(graphicsPipelineState);

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
	offScreenGraphicsPipelineState_.push_back(graphicsPipelineState);

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
	offScreenGraphicsPipelineState_.push_back(graphicsPipelineState);

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
	offScreenGraphicsPipelineState_.push_back(graphicsPipelineState);

}

void OffScreen::CreateRenderTargetView() {

	rtvIndex_ = rtvManager_->Allocate();

	offScreenRTVHandle_ = rtvManager_->GetCPUDescriptorHandle(rtvIndex_);

	rtvManager_->CreateRenderTargetView(rtvIndex_, renderTextureResrouce_.Get());
}

void OffScreen::CreateDepthStencilView() {

	offScreenDSVResrouce_ = CreateDepthStencilBuffer(
		dxCommon_->GetDevice(),
		WinApp::kClientWidth,
		WinApp::kClientHeight
	);

	dsvIndex_ = dsvManager_->Allocate();

	offScreenDSVHandle_ = dsvManager_->GetCPUDescriptorHandle(dsvIndex_);

	dsvManager_->CreateDepthStencilView(dsvIndex_, offScreenDSVResrouce_.Get());
}

void OffScreen::CreateShaderResourceView() {

	srvIndex_ = srvManager_->Allocate();

	offScreenSRVHandle_ = srvManager_->GetCPUDescriptorHandle(srvIndex_);

	srvManager_->CreateRenderTargetSRV(srvIndex_, renderTextureResrouce_.Get());
}

Microsoft::WRL::ComPtr<ID3D12Resource> OffScreen::CreateRenderTexture(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor) {

	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

	//リソースの情報
	D3D12_RESOURCE_DESC resourceDesc{};

	//リソースの設定
	resourceDesc.Width = width;										//ウィンドウの幅
	resourceDesc.Height = height;									//ウィンドウの高さ
	resourceDesc.MipLevels = 1;										//mipmapの数
	resourceDesc.DepthOrArraySize = 1;								//奥行き or 配列Textureの配列数
	resourceDesc.Format = format;									//DepthSthncilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;								//サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	//2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;	//RenderTargetとして使う通知

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; //VRAM上に作る

	//RenderTargetのクリア設定
	D3D12_CLEAR_VALUE renderTargetClearValue{};
	renderTargetClearValue.Color[0] = clearColor.x;
	renderTargetClearValue.Color[1] = clearColor.y;
	renderTargetClearValue.Color[2] = clearColor.z;
	renderTargetClearValue.Color[3] = clearColor.w;
	renderTargetClearValue.Format = format; //フォーマット。Resourceと合わせる

	//RenderTargetの初期化
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,					//Heapの設定
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定。特になし
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,		//Present状態にしておく
		&renderTargetClearValue,			//Clear最適値
		IID_PPV_ARGS(&resource)
	);

	assert(SUCCEEDED(hr));

	return resource;
}

Microsoft::WRL::ComPtr<ID3D12Resource> OffScreen::CreateDepthStencilBuffer(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t width, uint32_t height) {

	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

	//リソースの情報
	D3D12_RESOURCE_DESC resourceDesc{};

	//リソースの設定
	resourceDesc.Width = WinApp::kClientWidth;                    //ウィンドウの幅
	resourceDesc.Height = WinApp::kClientHeight;                  //ウィンドウの高さ
	resourceDesc.MipLevels = 1;                                   //mipmapの数
	resourceDesc.DepthOrArraySize = 1;                            //奥行き or 配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;          //DepthSthncilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;                            //サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;  //2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; //DepthStencilとして使う通知

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; //VRAM上に作る

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;              //1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //フォーマット。Resourceと合わせる

	//深度バッファの初期化
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,                  //Heapの設定
		D3D12_HEAP_FLAG_NONE,             //Heapの特殊な設定。特になし
		&resourceDesc,                    //Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //深度値を書き込む状態にしておく
		&depthClearValue,                 //Clear最適値
		IID_PPV_ARGS(&resource)
	);

	assert(SUCCEEDED(hr));

	return resource;
}
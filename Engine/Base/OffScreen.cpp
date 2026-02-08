#include "OffScreen.h"

#include "Base/WinApp.h"
#include "Base/DirectXCommon.h"
#include "Base/RTVManager.h"
#include "Base/DSVManager.h"
#include "Base/SrvManager.h"
#include "3d/Camera/Camera.h"
#include <Math/Utility/MakeMatrixMath.h>
#include "Other/Log.h"

#include "imgui.h"

#include "cassert"

namespace MyEngine {

	///=====================================================/// 
	/// OffScreenのシングルトンインスタンスを取得
	///=====================================================///
	OffScreen* OffScreen::GetInstance() {
		static OffScreen instance;
		return &instance;
	}

	///=====================================================/// 
	/// オフスクリーン描画用のリソースやパイプラインを初期化
	///=====================================================///
	void OffScreen::Initialize() {

		/// === インスタンスの取得 === ///

		//DirectX基底のインスタンスを取得
		dxCommon_ = DirectXCommon::GetInstance();

		//RTVマネージャーのインスタンスを取得
		rtvManager_ = RTVManager::GetInstance();

		//DSVマネージャーのインスタンスを取得
		dsvManager_ = DSVManager::GetInstance();

		//SRVマネージャーのインスタンスを取得
		srvManager_ = SrvManager::GetInstance();

		//CopyImageシェーダーを初期値に設定
		currentShaderName_ = L"CopyImage";

		/// === テクスチャの生成 === ///

		//レンダーテクスチャの生成
		renderTextureResrouce_ = CreateRenderTexture(
			dxCommon_->GetDevice(),
			WinApp::kClientWidth,
			WinApp::kClientHeight,
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			offScreenClearColor_
		);

		//深度テクスチャの生成
		depthTextureResource_ = CreateDepthTexture(
			dxCommon_->GetDevice(),
			WinApp::kClientWidth,
			WinApp::kClientHeight,
			DXGI_FORMAT_D24_UNORM_S8_UINT
		);

		/// === マテリアルの生成 === ///

		//マテリアルリソースを作成
		materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));

		//書き込むためのアドレスを取得する
		materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

		materialData_->projectionInverse = MakeIdentity4x4();

		materialData_->colorReverseRatio = 0.0f;

		/// === Viewの生成 === ///

		//RTVの生成
		CreateRenderTargetView();

		//DSVの生成
		CreateDepthStencilView();

		//RenderTextureのSRVを生成
		CreateRenderTextureSRV();

		//DepthTextureのSRVを生成
		CreateDepthTextureSRV();

		/// === パイプライン === ///

		//パイプラインの生成
		CreatePipeline();
	}

	///=====================================================/// 
	/// オフスクリーン描画用のレンダーターゲットと深度バッファの描画準備
	///=====================================================///
	void OffScreen::PreDraw() {

		D3D12_RESOURCE_BARRIER barriers[2]{};

		/// === RenderTextureのバリア === ///

		//バリアはTransitionタイプ
		barriers[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

		//フラグはNone
		barriers[0].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

		//バリアを張る対象のリソース。現在のバックバッファに対して行う
		barriers[0].Transition.pResource = renderTextureResrouce_.Get();

		//現在のバッファの状態を設定
		barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		//バッファの次の命令を描画状態に設定
		barriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		/// === DepthTextureのバリア === ///

		//バリアはTransitionタイプ
		barriers[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

		//フラグはNone
		barriers[1].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

		//バリアを張る対象のリソース。現在のバックバッファに対して行う
		barriers[1].Transition.pResource = depthTextureResource_.Get();

		//現在のバッファの状態を設定
		barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		//バッファの次の命令を描画状態に設定
		barriers[1].Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;

		//TransitionBarrierを張る
		dxCommon_->GetCommandList()->ResourceBarrier(2, barriers);

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

		//画面情報のクリアする
		dxCommon_->GetCommandList()->ClearRenderTargetView(offScreenRTVHandle_, clearColor, 0, nullptr);

		//深度情報のクリアする
		dxCommon_->GetCommandList()->ClearDepthStencilView(offScreenDSVHandle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		/// === 描画範囲の設定 === ///

		D3D12_VIEWPORT viewport = dxCommon_->GetViewport(); //Viewportを取得

		D3D12_RECT scissorRect = dxCommon_->GetScissorRect(); //Scissorを取得

		//Viewportを設定
		dxCommon_->GetCommandList()->RSSetViewports(1, &viewport);

		//Scissor設定
		dxCommon_->GetCommandList()->RSSetScissorRects(1, &scissorRect);

	}

	///==============================================================================/// 
	/// オフスクリーン描画後にレンダーターゲットと深度バッファをシェーダーで使用できる状態に戻す
	///==============================================================================///
	void OffScreen::PostDraw() {

		D3D12_RESOURCE_BARRIER barriers[2]{};

		/// === RenderTextureのバリア === ///

		//バリアのタイプはTransition
		barriers[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

		//フラグはNone
		barriers[0].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

		//バリアを張る対象のリソース。現在のバックバッファに対して行う
		barriers[0].Transition.pResource = renderTextureResrouce_.Get();

		//現在のバッファの状態を設定
		barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;

		//テクスチャとして使える状態に設定
		barriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		/// === DepthTextureのバリア === ///

		//バリアはTransitionタイプ
		barriers[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

		//フラグはNone
		barriers[1].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

		//バリアを張る対象のリソース。現在のバックバッファに対して行う
		barriers[1].Transition.pResource = depthTextureResource_.Get();

		//現在のバッファの状態を設定
		barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE;

		//バッファの次の命令を描画状態に設定
		barriers[1].Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		//TransitionBarrierを張る
		dxCommon_->GetCommandList()->ResourceBarrier(2, barriers);
	}

	///================================================================/// 
	/// オフスクリーンで描画した内容をスワップチェーンのバックバッファに転送して描画
	///================================================================///
	void OffScreen::DrawToSwapChain() {

		//ルートシグネチャの設定
		dxCommon_->GetCommandList()->SetGraphicsRootSignature(offScreenRootSignature_.Get());

		//PSOの設定
		dxCommon_->GetCommandList()->SetPipelineState(offScreenGraphicsPipelineState_[0].Get());

		//メッシュトポロジーを設定
		dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//RenderTextureのSRVの設定
		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(0, srvManager_->GetGPUDescriptorHandle(renderTextureSRVIndex_));

		//DepthTextureのSRVの設定
		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(depthTextureSRVIndex_));

		//マテリアルデータの設定
		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, materialResource_->GetGPUVirtualAddress());

		//描画命令
		dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
	}

	///=====================================================/// 
	/// ImGui
	///=====================================================///
	void OffScreen::ImGui() {

#ifdef _USE_IMGUI

		ImGui::Begin("OffScreen");

		if (ImGui::BeginTabBar("OffScreenTab")) {

			if (ImGui::BeginTabItem("パラメータ")) {

				ImGui::DragFloat("反転比率", &materialData_->colorReverseRatio, 0.01f, 0.0f, 1.0f);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("シェーダー")) {

				if (ImGui::Button("None")) {

					//現在のシェーダーをCopyImageに設定
					currentShaderName_ = L"CopyImage";

					//パイプラインを再生成
					CreatePipeline();
				}

				if (ImGui::Button("GrayScale")) {

					//現在のシェーダーをGrayScaleに設定
					currentShaderName_ = L"GrayScale";

					//パイプラインを再生成
					CreatePipeline();
				}

				if (ImGui::Button("Vignette")) {

					//現在のシェーダーをVignetteに設定
					currentShaderName_ = L"Vignette";

					//パイプラインを再生成
					CreatePipeline();
				}

				if (ImGui::Button("Smooth")) {

					//現在のシェーダーをSmoothに設定
					currentShaderName_ = L"Smooth";

					//パイプラインを再生成
					CreatePipeline();
				}

				if (ImGui::Button("OutLine")) {

					//現在のシェーダーをOutLineに設定
					currentShaderName_ = L"OutLine";

					//パイプラインを再生成
					CreatePipeline();
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();

#endif // _USE_IMGUI

	}

	///=====================================================/// 
	/// オフスクリーン描画用のルートシグネチャを作成
	///=====================================================///
	void OffScreen::CreateRootSignature() {

		HRESULT hr;

		//RootSignatureを作成
		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		/// === DescriptorRangeを作成 === ///

		D3D12_DESCRIPTOR_RANGE textureRange[1] = {};
		textureRange[0].BaseShaderRegister = 0; //0から始まる
		textureRange[0].NumDescriptors = 1; //数は1つ
		textureRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
		textureRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //Offsetを自動計算

		D3D12_DESCRIPTOR_RANGE depthTextureRange[1] = {};
		depthTextureRange[0].BaseShaderRegister = 1; //1から始まる
		depthTextureRange[0].NumDescriptors = 1; //数は1つ
		depthTextureRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
		depthTextureRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //Offsetを自動計算

		/// === RootParameterを作成 === ///

		D3D12_ROOT_PARAMETER rootParameters[3] = {};

		//テクスチャ
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   //DesctiptorTableを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;             //PixelShaderを使う
		rootParameters[0].DescriptorTable.pDescriptorRanges = textureRange;             //Tableの中身の配列を指定
		rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(textureRange); //Tableで利用する数
		rootParameters[0].Descriptor.RegisterSpace = 0;                                 //レジスタ番号0を使う

		//Depthテクスチャ
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;        //DesctiptorTableを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                  //PixelShaderを使う
		rootParameters[1].DescriptorTable.pDescriptorRanges = depthTextureRange;             //Tableの中身の配列を指定
		rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(depthTextureRange); //Tableで利用する数
		rootParameters[1].Descriptor.RegisterSpace = 1;                                      //レジスタ番号1を使う

		//マテリアル
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    //CBVを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderを使う
		rootParameters[2].Descriptor.ShaderRegister = 0;                    //レジスタ番号0とバインド

		descriptionRootSignature.pParameters = rootParameters;                             //ルートパラメータ配列へのポインタ
		descriptionRootSignature.NumParameters = _countof(rootParameters);                 //配列の長さ

		/// === Samplerを作成 === ///

		D3D12_STATIC_SAMPLER_DESC staticSamplers[2] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         //バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       //0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;     //比較しない
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       //ありったけのMinMapを使う
		staticSamplers[0].ShaderRegister = 0;                               //レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う

		staticSamplers[1].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;          //ポイントフィルタ
		staticSamplers[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       //0~1の範囲外をリピート
		staticSamplers[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[1].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;     //比較しない
		staticSamplers[1].MaxLOD = D3D12_FLOAT32_MAX;                       //ありったけのMinMapを使う
		staticSamplers[1].ShaderRegister = 1;                               //レジスタ番号1を使う
		staticSamplers[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderで使う

		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		/// === RootSignatureを作成 === ///

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

	///=====================================================/// 
	/// オフスクリーン描画用のグラフィックスパイプラインを生成
	///=====================================================///
	void OffScreen::CreatePipeline() {

		HRESULT hr;

		offScreenGraphicsPipelineState_.clear();

		/// === RootSignatureを設定する === ///

		//ルートシグネチャを生成
		CreateRootSignature();

		/// === InputLayoutを設定する === ///

		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};

		//VSに送るデータがないため設定はなし
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

		//シェーダーのディレクトリ
		const std::wstring shaderDirectory = L"Resource/Shader/OffScreen/";

		//現在のシェーダー名を元にファイル名を生成
		const std::wstring shaderFileName = shaderDirectory + currentShaderName_;

		//VertexShaderをコンパイルする
		Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob =
			dxCommon_->CompileShader(
				shaderDirectory + L"CopyImage.VS.hlsl",
				L"vs_6_0"
			);

		assert(vertexShaderBlob != nullptr);

		//PixelShaderをコンパイルする
		Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob =
			dxCommon_->CompileShader(
				shaderFileName + L".PS.hlsl",
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

	///=====================================================/// 
	/// オフスクリーン用のRenderTargetView(RTV)を生成
	///=====================================================///
	void OffScreen::CreateRenderTargetView() {

		//RTVのメモリを確保
		rtvIndex_ = rtvManager_->Allocate();

		//RTVのハンドルを取得
		offScreenRTVHandle_ = rtvManager_->GetCPUDescriptorHandle(rtvIndex_);

		//RTVを生成
		rtvManager_->CreateRenderTargetView(rtvIndex_, renderTextureResrouce_.Get());
	}

	///=====================================================/// 
	/// オフスクリーン用のDepthStencilView(DSV)を生成
	///=====================================================///
	void OffScreen::CreateDepthStencilView() {

		//DSVのメモリを確保
		dsvIndex_ = dsvManager_->Allocate();

		//DSVのハンドルを取得
		offScreenDSVHandle_ = dsvManager_->GetCPUDescriptorHandle(dsvIndex_);

		//DSVを生成
		dsvManager_->CreateDepthStencilView(dsvIndex_, depthTextureResource_.Get());
	}

	///=====================================================/// 
	/// オフスクリーン用のレンダーテクスチャSRVを生成
	///=====================================================///
	void OffScreen::CreateRenderTextureSRV() {

		//SRVのメモリを確保
		renderTextureSRVIndex_ = srvManager_->Allocate();

		//SRVのハンドルを取得
		renderTextureSRVHandle_ = srvManager_->GetCPUDescriptorHandle(renderTextureSRVIndex_);

		//SRVを生成
		srvManager_->CreateRenderTargetSRV(renderTextureSRVIndex_, renderTextureResrouce_.Get());
	}

	///=====================================================/// 
	/// 深度テクスチャ用のSRVを生成
	///=====================================================///
	void OffScreen::CreateDepthTextureSRV() {

		//SRVのメモリを確保
		depthTextureSRVIndex_ = srvManager_->Allocate();

		//SRVのハンドルを取得
		depthTextureSRVHandle_ = srvManager_->GetCPUDescriptorHandle(depthTextureSRVIndex_);

		//SRVを生成
		srvManager_->CreateDepthTextureSRV(depthTextureSRVIndex_, depthTextureResource_.Get());
	}

	///=====================================================/// 
	/// 指定サイズ・フォーマットのレンダーターゲット用テクスチャを生成
	///=====================================================///
	Microsoft::WRL::ComPtr<ID3D12Resource> OffScreen::CreateRenderTexture(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor) {

		Microsoft::WRL::ComPtr<ID3D12Resource> resource;

		//リソースの情報
		D3D12_RESOURCE_DESC resourceDesc{};

		//リソースの設定
		resourceDesc.Width = width;										//ウィンドウの幅
		resourceDesc.Height = height;									//ウィンドウの高さ
		resourceDesc.MipLevels = 1;										//mipmapの数
		resourceDesc.DepthOrArraySize = 1;								//奥行き or 配列Textureの配列数
		resourceDesc.Format = format;									//フォーマット
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
			&heapProperties,							//Heapの設定
			D3D12_HEAP_FLAG_NONE,						//Heapの特殊な設定。特になし
			&resourceDesc,								//Resourceの設定
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,	//ShaderResource状態にしておく
			&renderTargetClearValue,					//Clear最適値
			IID_PPV_ARGS(&resource)
		);

		assert(SUCCEEDED(hr));

		return resource;
	}

	///=====================================================/// 
	/// 指定サイズ・フォーマットの深度ステンシル用テクスチャを生成
	///=====================================================///
	Microsoft::WRL::ComPtr<ID3D12Resource> OffScreen::CreateDepthTexture(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format) {

		Microsoft::WRL::ComPtr<ID3D12Resource> resource;

		//リソースの情報
		D3D12_RESOURCE_DESC resourceDesc{};

		//リソースの設定
		resourceDesc.Width = width;										//ウィンドウの幅
		resourceDesc.Height = height;									//ウィンドウの高さ
		resourceDesc.MipLevels = 1;										//mipmapの数
		resourceDesc.DepthOrArraySize = 1;								//奥行き or 配列Textureの配列数
		resourceDesc.Format = format;									//フォーマット
		resourceDesc.SampleDesc.Count = 1;								//サンプリングカウント。1固定
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	//2次元
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//DepthStencilとして使う通知

		//利用するHeapの設定
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; //VRAM上に作る

		//DepthStencilのクリア設定
		D3D12_CLEAR_VALUE depthClearValue{};
		depthClearValue.DepthStencil.Depth = 1.0f;
		depthClearValue.Format = format; //フォーマット。Resourceと合わせる

		//RenderTargetの初期化
		HRESULT hr = device->CreateCommittedResource(
			&heapProperties,							//Heapの設定
			D3D12_HEAP_FLAG_NONE,						//Heapの特殊な設定。特になし
			&resourceDesc,								//Resourceの設定
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,	//ShaderResource状態にしておく
			&depthClearValue,							//Clear最適値
			IID_PPV_ARGS(&resource)
		);

		assert(SUCCEEDED(hr));

		return resource;
	}

	///=====================================================/// 
	/// デフォルトカメラの設定
	///=====================================================///
	void OffScreen::SetDefaultCamera(Camera* ptr) {
		camera_ = ptr;

		materialData_->projectionInverse = Inverse4x4(camera_->GetProjectionMatrix());
	}
}
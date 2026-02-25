#include "ImGuiManager.h"

#include "Base/WinApp.h"
#include "Base/DirectXCommon.h"

namespace MyEngine {

	///=====================================================/// 
	/// ImGuiを初期化
	///=====================================================///
	void ImGuiManager::Initialize(WinApp* winAppPtr, DirectXCommon* dxCommonPtr) {

#ifdef _USE_IMGUI

		HRESULT hr;

		//ウィンドウ管理のインスタンスを取得
		winApp_ = winAppPtr;

		//DirectX基底のインスタンスを取得
		dxCommon_ = dxCommonPtr;

		//デスクリプタヒープ設定
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};

		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.NumDescriptors = 1;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		hr = dxCommon_->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));

		assert(SUCCEEDED(hr));

		//ImGuiの初期化
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(winApp_->GetHwnd());
		ImGui_ImplDX12_Init(
			dxCommon_->GetDevice(),
			static_cast<int>(dxCommon_->GetBackBufferCount()),
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			srvHeap_.Get(),
			srvHeap_->GetCPUDescriptorHandleForHeapStart(),
			srvHeap_->GetGPUDescriptorHandleForHeapStart()
		);

		ImGuiIO& io = ImGui::GetIO();

		//日本語フォントを読み込み
		ImFont* japaneseFont = io.Fonts->AddFontFromFileTTF(
			"Resource/Font/Mplus1Code-Regular.ttf",
			16.0f,
			nullptr,
			io.Fonts->GetGlyphRangesJapanese()
		);

		//日本語フォントをデフォルトにする
		io.FontDefault = japaneseFont;

#endif // _USE_IMGUI

	}

	///=====================================================/// 
	/// ImGuiのリソースを解放し、終了処理を行う
	///=====================================================///
	void ImGuiManager::Finalize() {

#ifdef _USE_IMGUI

		//ImGuiの終了処理
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		//デスクリプタヒープの解放
		srvHeap_.Reset();

#endif // _USE_IMGUI

	}

	///==========================================================/// 
	/// mGuiの描画コマンドを生成し、DirectX 12のコマンドリストに積み込む
	///==========================================================///
	void ImGuiManager::Draw() {

#ifdef _USE_IMGUI

		ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

		//デスクリプタヒープの配列をセットする
		ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
		commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		//実際のcommandListのImGuiの描画コマンドを積む
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

#endif // _USE_IMGUI

	}

	///=====================================================/// 
	/// ImGuiの新しいフレームを開始
	///=====================================================///
	void ImGuiManager::Begin() {

#ifdef _USE_IMGUI

		//ここからImGuiのフレームが始まる
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

#endif // _USE_IMGUI

	}

	///=====================================================/// 
	/// ImGuiのフレームを終了し、描画用コマンドを生成
	///=====================================================///
	void ImGuiManager::End() {

#ifdef _USE_IMGUI

		//ImGuiの内部コマンドを生成する
		ImGui::Render();

#endif // _USE_IMGUI

	}
}
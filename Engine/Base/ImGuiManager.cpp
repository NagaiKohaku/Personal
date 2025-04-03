#include "ImGuiManager.h"

#include "Base/WinApp.h"
#include "Base/DirectXCommon.h"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
ImGuiManager* ImGuiManager::GetInstance() {
	static ImGuiManager instance;
	return &instance;
}

///=====================================================/// 
/// 初期化処理
///=====================================================///
void ImGuiManager::Initialize() {

	HRESULT hr;

	//ウィンドウ管理のインスタンスを取得
	winApp_ = WinApp::GetInstance();

	//DirectX基底のインスタンスを取得
	directXCommon_ = DirectXCommon::GetInstance();

	//デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};

	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	hr = directXCommon_->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));

	assert(SUCCEEDED(hr));

	//ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp_->GetHwnd());
	ImGui_ImplDX12_Init(
		directXCommon_->GetDevice(),
		static_cast<int>(directXCommon_->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);

}

///=====================================================/// 
/// 終了処理
///=====================================================///
void ImGuiManager::Finalize() {

	//ImGuiの終了処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//デスクリプタヒープの解放
	srvHeap_.Reset();
}

///=====================================================/// 
/// 描画処理
///=====================================================///
void ImGuiManager::Draw() {

	ID3D12GraphicsCommandList* commandList = directXCommon_->GetCommandList();

	//デスクリプタヒープの配列をセットする
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

///=====================================================/// 
/// 受付開始
///=====================================================///
void ImGuiManager::Begin() {

	//ここからImGuiのフレームが始まる
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}

///=====================================================/// 
/// 受付終了
///=====================================================///
void ImGuiManager::End() {

	//ImGuiの内部コマンドを生成する
	ImGui::Render();

}
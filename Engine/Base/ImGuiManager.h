#pragma once
#include "d3d12.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include "wrl.h"

/// === 前方宣言 === ///
class WinApp;

class DirectXCommon;

///=====================================================/// 
/// ImGuiマネージャー
///=====================================================///
class ImGuiManager {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static ImGuiManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// 受付終了
	/// </summary>
	void End();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//ウィンドウ管理
	WinApp* winApp_;

	//DirectX基底
	DirectXCommon* directXCommon_;

	//SRVヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};
#pragma once
#include "d3d12.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include "wrl.h"

/// === 前方宣言 === ///
class WinApp;

class DirectXCommon;

/// <summary>
/// ImGuiを管理・描画するクラスです。
/// </summary>
/// <remarks>
/// - シングルトンパターンで実装され、アプリケーション全体で1つのインスタンスを共有します。  
/// - ImGui コンテキストの生成、描画コマンドの生成を行います。  
/// - DirectX 12 用のシェーダー用デスクリプタヒープを作成し、描画時にコマンドリストにセットします。  
/// - Begin() / End() によりフレーム単位でGUI描画を制御できます。  
/// - Draw() により生成済みの描画コマンドをコマンドリストに積み込み、画面上にGUIを描画します。  
/// - Initialize() でフォント読み込みやWin32/DX12へのバインドを行い、Finalize() でリソース解放します。  
/// - 日本語フォント対応済みで、デフォルトフォントとして設定可能です。  
/// </remarks>
class ImGuiManager {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// ImGuiManagerのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static ImGuiManager* GetInstance();

	/// <summary>
	/// ImGuiを初期化します。
	/// </summary>
	/// <remarks>
	/// - ウィンドウ管理とDirectX基底のインスタンスを取得します。  
	/// - シェーダーから参照可能なデスクリプタヒープを1つ作成します。  
	/// - ImGuiコンテキストを生成します。  
	/// - ImGuiをWin32とDX12にバインドします。  
	/// - 日本語フォントを読み込み、デフォルトフォントとして設定します。  
	/// </remarks>
	void Initialize();

	/// <summary>
	/// ImGuiのリソースを解放し、終了処理を行います。
	/// </summary>
	/// <remarks>
	/// - DirectX 12およびWin32向けのImGuiバックエンドを終了します。  
	/// - ImGuiのコンテキストを破棄します。  
	/// - 作成したシェーダー用デスクリプタヒープを解放します。  
	/// </remarks>
	void Finalize();

	/// <summary>
	/// ImGuiの描画コマンドを生成し、DirectX 12のコマンドリストに積み込みます。
	/// </summary>
	/// <remarks>
	/// - 現在のコマンドリストを取得します。  
	/// - ImGui用に作成したシェーダー用デスクリプタヒープをコマンドリストにセットします。  
	/// - ImGuiが生成した描画データをコマンドリストに積むことで、画面にGUIを描画可能にします。  
	/// </remarks>
	void Draw();

	/// <summary>
	/// ImGuiの新しいフレームを開始します。
	/// </summary>
	/// <remarks>
	/// - DirectX 12用のImGuiフレームを開始します。  
	/// - Win32用のImGuiフレームを開始します。  
	/// - ImGuiの内部で新しいフレームを生成し、GUI描画の準備を行います。  
	/// </remarks>
	void Begin();

	/// <summary>
	/// ImGuiのフレームを終了し、描画用コマンドを生成します。
	/// </summary>
	/// <remarks>
	/// - ImGuiの内部で描画コマンドを生成します。  
	/// - これにより、Draw() でコマンドリストにGUI描画命令を積む準備が整います。  
	/// </remarks>
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
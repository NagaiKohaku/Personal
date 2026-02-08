#pragma once
#include "d3d12.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include "wrl.h"

namespace MyEngine {

	/// === 前方宣言 === ///
	class WinApp;

	class DirectXCommon;

	/// <summary>
	/// ImGuiを管理・描画するクラスです。
	/// </summary>
	class ImGuiManager {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// ImGuiManagerのシングルトンインスタンスを取得します。
		/// </summary>
		static ImGuiManager* GetInstance();

		/// <summary>
		/// ImGuiを初期化します。
		/// </summary>
		void Initialize();

		/// <summary>
		/// ImGuiのリソースを解放し、終了処理を行います。
		/// </summary>
		void Finalize();

		/// <summary>
		/// ImGuiの描画コマンドを生成し、DirectX 12のコマンドリストに積み込みます。
		/// </summary>
		void Draw();

		/// <summary>
		/// ImGuiの新しいフレームを開始します。
		/// </summary>
		void Begin();

		/// <summary>
		/// ImGuiのフレームを終了し、描画用コマンドを生成します。
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
}
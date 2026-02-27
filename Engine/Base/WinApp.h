#pragma once
#include "Windows.h"

#include "cstdint"

#pragma comment(lib,"winmm.lib")

namespace MyEngine {

	/// <summary>
	/// OSのウィンドウ管理を行う仕組みです。
	/// </summary>
	class WinApp {

	public:

		WinApp();

		~WinApp();

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// OSから送られてくるウィンドウメッセージを処理します。
		/// </summary>
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		/// <summary>
		/// ウィンドウを初期化して表示します。
		/// </summary>
		void Initialize();

		/// <summary>
		/// ウィンドウを閉じ、COMを解放します。
		/// </summary>
		void Finalize();

		/// <summary>
		/// ウィンドウに送られてきたメッセージを取得して処理します。
		/// </summary>
		bool ProcessMessage();

		///-------------------------------------------/// 
		/// メンバ定数
		///-------------------------------------------///
	public:

		//ウィンドウの横幅
		const int32_t kWindowWidth_ = 1280;

		//ウィンドウの縦幅
		const int32_t kWindowHeight_ = 720;

		//ウィンドウのタイトル
		const wchar_t* kWindowTitle_ = L"LE3B_21_ナガイ_コハク_ロードフライト";

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//ウィンドウクラス
		WNDCLASS wc_{};

		//ウィンドウハンドル
		HWND hwnd_ = nullptr;

		///-------------------------------------------/// 
		/// ゲッター・セッター
		///-------------------------------------------///
	public:

		/// <summary>
		/// インスタンスハンドルを取得
		/// </summary>
		/// <returns>インスタンスハンドル</returns>
		HINSTANCE GetHInstance() const { return wc_.hInstance; }

		/// <summary>
		/// ウィンドウハンドルを取得
		/// </summary>
		/// <returns>ウィンドウハンドル</returns>
		HWND GetHwnd() const { return hwnd_; }

		/// <summary>
		/// ウィンドウの横幅を取得
		/// </summary>
		/// <returns>ウィンドウの横幅</returns>
		int32_t GetWindowWidth() const { return kWindowWidth_; }

		/// <summary>
		/// ウィンドウの縦幅を取得
		/// </summary>
		/// <returns>ウィンドウの縦幅</returns>
		int32_t GetWindowHeight() const { return kWindowHeight_; }

	};
}
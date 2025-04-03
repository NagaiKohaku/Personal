#pragma once
#include "Windows.h"

#include "cstdint"

#pragma comment(lib,"winmm.lib")

///=====================================================/// 
/// ウィンドウ管理
///=====================================================///
class WinApp {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static WinApp* GetInstance();

	/// <summary>
	/// ウィンドウ操作
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="msg">メッセージコード</param>
	/// <param name="wparam">ウィンドウのサイズ変更フラグ</param>
	/// <param name="lparam">ウィンドウのサイズ</param>
	/// <returns></returns>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// メッセージ処理
	/// </summary>
	/// <returns>アクションフラグ</returns>
	bool ProcessMessage();

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// インスタンスハンドルのゲッター
	/// </summary>
	/// <returns>インスタンスハンドル</returns>
	HINSTANCE GetHInstance() const { return wc_.hInstance; }

	/// <summary>
	/// ウィンドウハンドルのゲッター
	/// </summary>
	/// <returns>ウィンドウハンドル</returns>
	HWND GetHwnd() const { return hwnd_; }

	///-------------------------------------------/// 
	/// 静的メンバ変数
	///-------------------------------------------///
public:

	//ウィンドウの横幅
	static const int32_t kClientWidth = 1280;

	//ウィンドウの縦幅
	static const int32_t kClientHeight = 720;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//ウィンドウクラス
	WNDCLASS wc_{};

	//ウィンドウハンドル
	HWND hwnd_ = nullptr;

};
#include "WinApp.h"

#include "imgui.h"

#include "cassert"
#include "wrl.h"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
WinApp* WinApp::GetInstance() {
	static WinApp instance;
	return &instance;
}

///=====================================================/// 
/// imguiのウィンドウハンドル
///=====================================================///
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

///=====================================================/// 
/// ウィンドウ操作
///=====================================================///
LRESULT CALLBACK WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	//ImGuiのウィンドウを操作するときに通る
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {

		//ウィンドウが破棄された
	case WM_DESTROY:

		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

///=====================================================/// 
/// 初期化処理
///=====================================================///
void WinApp::Initialize() {

	///-------------------------------------------/// 
	/// ウィンドウクラスの設定
	///-------------------------------------------///

	//テクスチャのためのCOMの初期化
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);

	//ウィンドウプロシージャ
	wc_.lpfnWndProc = WindowProc;

	//ウィンドウクラス名
	wc_.lpszClassName = L"WindowClass";

	//インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);

	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wc_);

	///-------------------------------------------/// 
	/// ウィンドウの設定
	///-------------------------------------------///

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0,kClientWidth,kClientHeight };

	//クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの生成
	hwnd_ = CreateWindow(
		wc_.lpszClassName,           //利用するクラス名
		L"LE2C_18_ナガイ_コハク",     //タイトルバーの文字
		WS_OVERLAPPEDWINDOW,        //よく見るウィンドウスタイル
		CW_USEDEFAULT,              //表示X座標(Windowsに任せる)
		CW_USEDEFAULT,              //表示Y座標(WindowsOSに任せる)
		wrc.right - wrc.left,       //ウィンドウ横幅
		wrc.bottom - wrc.top,       //ウィンドウ縦幅
		nullptr,                    //親ウィンドウハンドル
		nullptr,                    //メニューハンドル
		wc_.hInstance,               //インスタンスハンドル
		nullptr                     //オプション
	);

	///-------------------------------------------/// 
	/// ウィンドウを生成
	///-------------------------------------------///

	//hwndの設定をもとにウィンドウを生成する
	ShowWindow(hwnd_, SW_SHOW);

	timeBeginPeriod(1);
}

///=====================================================/// 
/// 終了処理
///=====================================================///
void WinApp::Finalize() {

	//ウィンドウを閉じる
	CloseWindow(hwnd_);
	CoUninitialize();
}

///=====================================================/// 
/// メッセージ処理
///=====================================================///
bool WinApp::ProcessMessage() {

	//ウィンドウへのアクションの情報を受け取る
	MSG msg{};

	//Windowにメッセージが来てたら最優先で処理させる
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}
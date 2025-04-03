#include "Input.h"

#include "Base/WinApp.h"

#include "cassert"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

///=====================================================/// 
/// 初期化処理
///=====================================================///
void Input::Initialize() {

	HRESULT result;

	//ウィンドウ管理のインスタンスを取得
	winApp_ = WinApp::GetInstance();

	//DirectInputオブジェクトの生成
	result = DirectInput8Create(
		winApp_->GetHInstance(),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput_,
		nullptr
	);

	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	result = directInput_->CreateDevice(
		GUID_SysKeyboard,
		&keyboard_,
		NULL
	);

	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);

	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(
		winApp_->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);

	assert(SUCCEEDED(result));

	//マウスデバイスの生成
	result = directInput_->CreateDevice(
		GUID_SysMouse,
		&mouse_,
		NULL
	);

	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = mouse_->SetDataFormat(&c_dfDIMouse);

	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = mouse_->SetCooperativeLevel(
		winApp_->GetHwnd(),
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
	);

	assert(SUCCEEDED(result));

}

///=====================================================/// 
/// 更新処理
///=====================================================///
void Input::Update() {

	//前フレームのキーボード情報をコピー
	for (int i = 0; i < 256; i++) {

		preKey_[i] = key_[i];
	}

	//前フレームのマウス情報をコピー
	preMouseState_ = mouseState_;

	//キーボード情報の取得開始
	keyboard_->Acquire();

	//全キーの入力情報を取得する
	keyboard_->GetDeviceState(sizeof(key_), key_);

	//マウス情報の取得開始
	mouse_->Acquire();

	//ポーリング開始
	mouse_->Poll();

	//マウスの入力情報を取得する
	mouse_->GetDeviceState(sizeof(mouseState_), &mouseState_);
}

///=====================================================/// 
/// キーが押されているかの判定
///=====================================================///
bool Input::isPushKey(uint8_t keyNum) {

	//押されている場合trueを返す
	if (key_[keyNum]) {

		return true;
	}

	return false;
}

///=====================================================/// 
/// キーが離されているかの判定
///=====================================================///
bool Input::isReleaseKey(uint8_t keyNum) {

	//押されていない場合trueを返す
	if (!key_[keyNum]) {

		return true;
	}

	return false;
}

///=====================================================/// 
/// キーが押された瞬間かの判定
///=====================================================///
bool Input::IsTriggerPushKey(uint8_t keyNum) {

	//押された瞬間の場合trueを返す
	if (key_[keyNum] && !preKey_[keyNum]) {

		return true;
	}

	return false;
}

///=====================================================/// 
/// キーが離された瞬間かの判定
///=====================================================///
bool Input::IsTriggerReleaseKey(uint8_t keyNum) {

	//離された瞬間の場合trueを返す
	if (!key_[keyNum] && preKey_[keyNum]) {

		return true;
	}

	return false;
}

///=====================================================/// 
/// マウスボタンが押されているかの判定
///=====================================================///
bool Input::IsPushMouseButton(int mouseButton) {

	//マウスボタンの範囲内だったら
	if (mouseButton >= 0 && mouseButton <= 3) {

		//指定のマウスボタンが押されていたら
		if (mouseState_.rgbButtons[mouseButton]) {

			return true;
		}
	}

	return false;
}

///=====================================================/// 
/// マウスボタンが離されているかの判定
///=====================================================///
bool Input::IsReleaseMouseButton(int mouseButton) {

	//マウスボタンの範囲内だったら
	if (mouseButton >= 0 && mouseButton <= 3) {

		//指定のマウスボタンが押されていたら
		if (!mouseState_.rgbButtons[mouseButton]) {

			return true;
		}
	}

	return false;
}

///=====================================================/// 
/// マウスボタンが押された瞬間かの判定
///=====================================================///
bool Input::IsTriggerPushMouseButton(int mouseButton) {

	//マウスボタンの範囲内だったら
	if (mouseButton >= 0 && mouseButton <= 3) {

		//指定のマウスボタンが押されていたら
		if (mouseState_.rgbButtons[mouseButton] && !preMouseState_.rgbButtons[mouseButton]) {

			return true;
		}
	}

	return false;
}

///=====================================================/// 
/// マウスボタンが離された瞬間かの判定
///=====================================================///
bool Input::IsTriggerReleaseMouseButton(int mouseButton) {

	//マウスボタンの範囲内だったら
	if (mouseButton >= 0 && mouseButton <= 3) {

		//指定のマウスボタンが押されていたら
		if (!mouseState_.rgbButtons[mouseButton] && preMouseState_.rgbButtons[mouseButton]) {

			return true;
		}
	}

	return false;
}

///=====================================================/// 
/// マウス座標のゲッター
///=====================================================///
Vector2 Input::GetMousePos() {

	//マウス座標
	POINT point;

	//マウス座標を取得する
	GetCursorPos(&point);

	//ウィンドウサイズに合わせる
	ScreenToClient(winApp_->GetHwnd(), &point);

	return Vector2(static_cast<float>(point.x), static_cast<float>(point.y));
}

///=====================================================/// 
/// マウスの移動量のゲッター
///=====================================================///
Vector3 Input::GetMouseVelocity() {

	//マウスの移動量を返す
	return Vector3(static_cast<float>(mouseState_.lX), static_cast<float>(mouseState_.lY), static_cast<float>(mouseState_.lZ));
}
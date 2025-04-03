#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"
#include "Windows.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "Math/Vector2.h"
#include "Math/Vector3.h"

#include "cstdint"

#include "wrl.h"

/// === 前方宣言 === ///
class WinApp;

///=====================================================/// 
/// 入力
///=====================================================///
class Input {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static Input* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// キーが押されているか判定
	/// </summary>
	/// <param name="keyNum">キー番号</param>
	/// <returns>判定フラグ</returns>
	bool isPushKey(uint8_t keyNum);

	/// <summary>
	/// キーが離されているか判定
	/// </summary>
	/// <param name="keyNum">キー番号</param>
	/// <returns>判定フラグ</returns>
	bool isReleaseKey(uint8_t keyNum);

	/// <summary>
	/// キーが押された瞬間か判定
	/// </summary>
	/// <param name="keyNum">キー番号</param>
	/// <returns>判定フラグ</returns>
	bool IsTriggerPushKey(uint8_t keyNum);
	
	/// <summary>
	/// キーが離された瞬間か判定
	/// </summary>
	/// <param name="keyNum">キー番号</param>
	/// <returns>判定フラグ</returns>
	bool IsTriggerReleaseKey(uint8_t keyNum);

	/// <summary>
	/// マウスボタンを押しているか判定
	/// </summary>
	/// <param name="mouseButton">マウスボタン番号</param>
	/// <returns>判定フラグ</returns>
	bool IsPushMouseButton(int mouseButton);

	/// <summary>
	/// マウスボタンを離しているか判定
	/// </summary>
	/// <param name="mouseButton">マウスボタン番号</param>
	/// <returns>判定フラグ</returns>
	bool IsReleaseMouseButton(int mouseButton);

	/// <summary>
	/// マウスボタンを押した瞬間か判定
	/// </summary>
	/// <param name="mouseButton">マウスボタン番号</param>
	/// <returns>判定フラグ</returns>
	bool IsTriggerPushMouseButton(int mouseButton);

	/// <summary>
	/// マウスボタンを離した瞬間か判定
	/// </summary>
	/// <param name="mouseButton">マウスボタン番号</param>
	/// <returns>判定フラグ</returns>
	bool IsTriggerReleaseMouseButton(int mouseButton);

	///-------------------------------------------/// 
	/// セッター・ゲッター
	///-------------------------------------------///
public:

	/// <summary>
	/// マウス座標のゲッター
	/// </summary>
	/// <returns>マウス座標</returns>
	Vector2 GetMousePos();

	/// <summary>
	/// マウスの移動量のゲッター
	/// </summary>
	/// <returns>マウスの移動量</returns>
	Vector3 GetMouseVelocity();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//ウィンドウ管理
	WinApp* winApp_ = nullptr;

	//DirectInput
	Microsoft::WRL::ComPtr<IDirectInput8> directInput_ = nullptr;

	//キーボード入力
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_;

	//マウス入力
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse_;

	//ゲームパッド入力
	Microsoft::WRL::ComPtr<IDirectInputDevice8> gamePad_;

	//今フレームのキー入力
	BYTE key_[256] = {};

	//前フレームのキー入力
	BYTE preKey_[256] = {};

	//今フレームのマウス入力
	DIMOUSESTATE mouseState_;

	//前フレームのマウス入力
	DIMOUSESTATE preMouseState_;
};
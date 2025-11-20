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

/// <summary>
/// キーボード、マウスなどの入力デバイスを統括して管理するクラスです。
/// </summary>
/// <remarks>
/// - シングルトンパターンで実装され、アプリケーション全体で 1 つのインスタンスを共有します。  
/// - 初期化時にウィンドウ情報を取得し、各入力デバイスを生成・設定して使用可能な状態にします。  
/// - Update()を呼び出すことで、前フレームの入力状態を記録し、最新の入力状態を取得します。  
/// - キーやボタンが押されているか、離されているか、押した瞬間や離した瞬間かを判定できます。  
/// - マウス座標や移動量も取得可能で、ユーザー操作に応じた処理に利用できます。  
/// </remarks>
class Input {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// Inputのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static Input* GetInstance();

	/// <summary>
	/// DirectInputを使用してキーボードとマウスの入力デバイスを初期化します。
	/// </summary>
	/// <remarks>
	/// - ウィンドウ管理のインスタンスからアプリケーションハンドルを取得します。  
	/// - DirectInputオブジェクトを生成します。  
	/// - キーボードデバイスを生成し、データ形式と排他制御レベルを設定します。  
	/// - マウスデバイスを生成し、データ形式と排他制御レベルを設定します。  
	/// </remarks>
	void Initialize();

	/// <summary>
	/// 毎フレーム、キーボードとマウスの入力状態を更新します。
	/// </summary>
	/// <remarks>
	/// - 前フレームのキーボードとマウスの状態を保存します。  
	/// - キーボードデバイスを取得し、全キーの入力状態を取得します。  
	/// - マウスデバイスを取得し、ポーリングを行い、現在の入力状態を取得します。  
	/// - この更新により、押下・離上などの入力判定に利用可能な最新状態が保持されます。  
	/// </remarks>
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

	/// <summary>
	/// マウス座標を取得
	/// </summary>
	/// <returns>マウス座標</returns>
	Vector2 GetMousePos();

	/// <summary>
	/// マウスの移動量を取得
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
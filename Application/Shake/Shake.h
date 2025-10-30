#pragma once

#include "3d/Camera/Camera.h"
#include "Math/WorldTransform.h"

///=====================================================/// 
/// シェイククラス
///=====================================================///
class Shake {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static Shake* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// シェイク開始
	/// </summary>
	/// <param name="num">震える長さ</param>
	/// <param name="time">震える時間</param>
	void Start(float length, float time);

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// カメラを設定
	/// </summary>
	/// <param name="ptr">カメラポインタ</param>
	void SetCamera(Camera* ptr) { camera_ = ptr; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	Camera* camera_;

	//トランスフォーム
	WorldTransform transform_;

	//タイマー
	float timer_;

	//震える時間
	float shakeTime_;

	//震える長さ
	float shakeLength_;

};
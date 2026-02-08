#pragma once

#include "3d/Camera/Camera.h"
#include <Math/Transform/WorldTransform.h>

/// <summary>
/// カメラに対してシェイク処理を管理するクラス
/// </summary>
class Shake {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// Shakeのシングルトンインスタンスを取得します。
	/// </summary>
	static Shake* GetInstance();

	/// <summary>
	/// シェイク処理用の初期化を行う
	/// </summary>
	void Initialize();

	/// <summary>
	/// カメラに対してシェイク効果を適用する
	/// </summary>
	void Update();

	/// <summary>
	/// シェイクを開始し初期パラメータを設定する
	/// </summary>
	/// <param name="num">揺れの強さ</param>
	/// <param name="time">揺れの持続時間</param>
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
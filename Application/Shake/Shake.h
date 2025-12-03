#pragma once

#include "3d/Camera/Camera.h"
#include "Math/WorldTransform.h"

/// <summary>
/// カメラに対してシェイク処理を管理するクラス
/// </summary>
/// <remarks>
/// - シングルトンとしてインスタンスを取得できる
/// - 初期化・更新・開始の操作を提供する
/// - カメラの変換情報を元に揺れを加算する
/// - 揺れの強さや持続時間を制御可能
/// </remarks>
class Shake {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// Shakeのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static Shake* GetInstance();

	/// <summary>
	/// シェイク処理用の初期化を行う
	/// </summary>
	/// <remarks>
	/// - 変換情報を初期状態にリセットする
	/// - 揺れタイマーを0に設定する
	/// </remarks>
	void Initialize();

	/// <summary>
	/// カメラに対してシェイク効果を適用する
	/// </summary>
	/// <remarks>
	/// - カメラが存在しなければ処理をスキップする
	/// - 揺れタイマーを進めて経過率を計算する
	/// - 揺れの強さを徐々に0に減衰させる
	/// - カメラの変換情報をコピーし揺れ分を加算する
	/// - 変換行列を更新しビュー行列を計算する
	/// - ビュープロジェクション行列を計算してカメラに設定する
	/// </remarks>
	void Update();

	/// <summary>
	/// シェイクを開始し初期パラメータを設定する
	/// </summary>
	/// <param name="num">揺れの強さ</param>
	/// <param name="time">揺れの持続時間</param>
	/// <remarks>
	/// - タイマーをリセットする
	/// - 揺れの持続時間を設定する
	/// - 揺れの初期強さを設定する
	/// </remarks>
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
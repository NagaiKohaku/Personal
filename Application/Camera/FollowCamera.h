#pragma once

#include "3d/Camera/Camera.h"

#include "Player/Player.h"

///=====================================================/// 
/// 追従カメラクラス
///=====================================================///
class FollowCamera {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラポインタ</param>
	/// <param name="player">プレイヤーポインタ</param>
	void Initialize(Camera* camera, Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// 有効化フラグのセッター
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetIsActive(bool flag) { isActive_ = flag; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	Camera* camera_;

	//プレイヤー
	Player* player_;

	//オフセット
	Vector3 offset_;

	//追尾範囲
	float followRange_;

	//追尾強度
	float followStrength_;

	//有効化フラグ
	bool isActive_;
};
#pragma once

#include "3d/Camera/Camera.h"

#include <Object/Player/Player.h>

/// <summary>
/// プレイヤーに追従するカメラクラスです。
/// </summary>
class FollowCamera {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:
	/// <summary>
	/// カメラをプレイヤーに追従させるために初期化します。
	/// </summary>
	/// <param name="camera">カメラを指定します。</param>
	/// <param name="player">追従先となるプレイヤーを指定します。</param>
	void Initialize(MyEngine::Camera* camera, Player* player);

	/// <summary>
	/// プレイヤーにカメラを追従させるために毎フレーム更新します。
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
	MyEngine::Camera* camera_;

	//プレイヤー
	Player* player_;

	//オフセット
	MyEngine::Vector3 offset_;

	//追尾範囲
	float followRange_;

	//追尾強度(％)
	float followStrength_;

	//傾き強度(％)
	float slopeStrength_;

	//プレイヤーとカメラの距離
	float distanceToPlayer_;

	//有効化フラグ
	bool isActive_;
};
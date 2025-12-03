#pragma once

#include "3d/Camera/Camera.h"

#include "Player/Player.h"

/// <summary>
/// プレイヤーに追従するカメラクラスです。
/// </summary>
/// <remarks>
/// - プレイヤーの位置や移動方向に応じてカメラの方向を追従させます
/// - パラメータで追尾範囲や強度を調節可能
/// </remarks>
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
	/// <remarks>
	/// - カメラとプレイヤーのポインタを保持します。  
	/// - パラメータの値を設定します。
	/// - パラメータを基にカメラの座標と距離を設定します。  
	/// </remarks>
	void Initialize(Camera* camera, Player* player);

	/// <summary>
	/// プレイヤーにカメラを追従させるために毎フレーム更新します。
	/// </summary>
	/// <remarks>
	/// - デバッグカメラの場合や無効化されている場合は処理を行いません。  
	/// - カメラとプレイヤーの位置関係からカメラの角度を計算します。
	/// </remarks>
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

	//追尾強度(％)
	float followStrength_;

	//傾き強度(％)
	float slopeStrength_;

	//プレイヤーとカメラの距離
	float distanceToPlayer_;

	//有効化フラグ
	bool isActive_;
};
#pragma once

#include "3d/Object/Object3D.h"

#include "memory"

///=====================================================/// 
/// バレットベースクラス
///=====================================================///
class BulletBase {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BulletBase() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos">初期座標</param>
	/// <param name="direction">発射方向</param>
	virtual void Initialize(Vector3 pos, Vector3 direction) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 座標のみ更新
	/// </summary>
	virtual void TransformUpdate() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
protected:

	//オブジェクト
	std::unique_ptr<Object3D> object_ = nullptr;

	//移動速度
	float speed_ = 0.0f;

	//移動量
	Vector3 velocity_ = { 0.0f,0.0f,0.0f };

	//弾の寿命
	float lifeTimer_ = 0.0f;

	//弾の最大寿命
	float lifeTimeMax_ = 0.0f;

	//消滅フラグ
	bool isDead_ = false;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// 消滅フラグを取得
	/// </summary>
	/// <returns>フラグ</returns>
	bool IsDead() const { return isDead_; }
};
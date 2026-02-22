#pragma once

#include "3d/Object/Object3D.h"

#include "3d/Collider/SphereCollider.h"

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
	virtual void Initialize(MyEngine::Vector3 pos, MyEngine::Vector3 direction);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 座標のみ更新
	/// </summary>
	virtual void TransformUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

protected:

	/// <summary>
	/// 弾の移動処理を行います
	virtual void Move();

	/// <summary>
	/// 他オブジェクトと接触している場合の処理を行います
	/// </summary>
	virtual void IsCollision();

protected:

	virtual MyEngine::Vector3 GetObjectSize() const = 0;

	virtual float GetColliderSize() const = 0;

	virtual float GetSpeed() const = 0;

	virtual float GetLifeTime() const = 0;

	virtual MyEngine::Collider::Tag GetColliderTag() const = 0;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
protected:

	//オブジェクト
	std::unique_ptr<MyEngine::Object3D> object_ = nullptr;

	//コライダー
	std::unique_ptr<MyEngine::SphereCollider> collider_;

	//移動速度
	float speed_;

	//移動量
	MyEngine::Vector3 velocity_;

	//弾の寿命
	float lifeTimer_;

	//弾の最大寿命
	float lifeTimeMax_;

	//消滅フラグ
	bool isDead_;

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
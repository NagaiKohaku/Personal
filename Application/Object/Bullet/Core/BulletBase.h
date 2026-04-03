#pragma once

#include <Object/3D/Object3D.h>

#include "3d/Collider/SphereCollider.h"

#include <Scene/EngineContext.h>

#include "memory"

///=====================================================/// 
/// バレットベースクラス
///=====================================================///
class BulletBase {

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BulletBase();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos">初期座標</param>
	/// <param name="direction">発射方向</param>
	void Initialize(EngineContext context, MyEngine::Vector3 pos, MyEngine::Vector3 direction);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 座標のみ更新
	/// </summary>
	void TransformUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

protected:

	/// <summary>
	/// 弾の移動処理を行います
	void Move();

	/// <summary>
	/// 他オブジェクトと接触している場合の処理を行います
	/// </summary>
	virtual void IsCollision() = 0;

protected:

	/// <summary>
	/// 派生クラスのオブジェクトのサイズ情報を取得
	/// </summary>
	/// <returns>オブジェクトのサイズ</returns>
	virtual MyEngine::Vector3 GetObjectSize() const = 0;

	/// <summary>
	/// 派生クラスのコライダーのサイズ情報を取得
	/// </summary>
	/// <returns>コライダーのサイズ</returns>
	virtual float GetColliderSize() const = 0;

	/// <summary>
	/// 派生クラスの移動速度情報を取得
	/// </summary>
	/// <returns>移動速度</returns>
	virtual float GetSpeed() const = 0;

	/// <summary>
	/// 派生クラスの生存時間情報を取得
	/// </summary>
	/// <returns>生存時間</returns>
	virtual float GetLifeTime() const = 0;

	/// <summary>
	/// 派生クラスのコライダーのタグ情報を取得
	/// </summary>
	/// <returns>コライダーのタグ</returns>
	virtual MyEngine::Collider::Tag GetColliderTag() const = 0;

protected:

	//オブジェクト
	MyEngine::Object3D* object_ = nullptr;

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

public:

	/// <summary>
	/// 消滅フラグを取得
	/// </summary>
	/// <returns>フラグ</returns>
	bool IsDead() const { return isDead_; }
};
#pragma once

#include <Object/Bullet/Core/BulletBase.h>

#include "3d/Collider/SphereCollider.h"

#include "memory"

class GroundBullet : public BulletBase {

private:

	void Update() override;

	/// <summary>
	/// 他オブジェクトと接触している場合の処理を行います
	/// </summary>
	void IsCollision() override;

	void Draw() override;

private:

	//オブジェクトのサイズ
	const MyEngine::Vector3 objectSize_ = { 0.5f,0.5f,0.5f };

	//コライダーのサイズ
	const float colliderSize_ = 0.5f;

	//移動速度
	const float speed_ = 0.5f;

	//生存時間
	const float lifeTime_ = 2.0f;

	//コライダーのタグ
	const MyEngine::Collider::Tag colliderTag_ = MyEngine::Collider::Tag::ENEMYBULLET;

	float timer_ = 0.0f;

	bool beenCreated_ = false;

private:

	void CreateDamageGround();

	/// <summary>
	/// 基底クラスにオブジェクトのサイズ情報取得させる
	/// </summary>
	/// <returns>オブジェクトのサイズ</returns>
	MyEngine::Vector3 GetObjectSize() const override { return objectSize_; }

	/// <summary>
	/// 基底クラスにコライダーのサイズ情報取得させる
	/// </summary>
	/// <returns>コライダーのサイズ</returns>
	float GetColliderSize() const override { return colliderSize_; }

	/// <summary>
	/// 基底クラスに移動速度情報取得させる
	/// </summary>
	/// <returns>移動速度</returns>
	float GetSpeed() const override { return speed_; }

	/// <summary>
	/// 基底クラスに生存時間情報取得させる
	/// </summary>
	/// <returns>生存時間</returns>
	float GetLifeTime() const override { return lifeTime_; }

	/// <summary>
	/// 基底クラスにコライダーのタグ情報取得させる
	/// </summary>
	/// <returns>コライダーのタグ</returns>
	MyEngine::Collider::Tag GetColliderTag() const override { return colliderTag_; }

};


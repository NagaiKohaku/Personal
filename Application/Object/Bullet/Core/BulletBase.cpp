#include "BulletBase.h"

using namespace MyEngine;

///=====================================================/// 
/// 弾が破棄される際に関連するコライダーを削除
///=====================================================///
BulletBase::~BulletBase() {

	//コライダーを削除
	collider_->Remove();
}

///=====================================================/// 
/// 弾を初期化し、パラメータを設定
///=====================================================///
void BulletBase::Initialize(EngineContext context, Vector3 pos, Vector3 direction) {

	/// === オブジェクトの生成 === ///

	//生成
	object_ = std::make_unique<Object3D>();

	//初期化
	object_->Initialize(context.objectCommon.object3DCommon, context.objectCommon.debugObjectCommon);

	//座標の設定
	object_->GetWorldTransform().translate_ = pos;

	//サイズの設定
	object_->GetWorldTransform().scale_ = GetObjectSize();

	//モデルの設定
	object_->SetModel("Sphere");

	/// === コライダーの生成 === ///

	//生成
	collider_ = std::make_unique<SphereCollider>();

	//初期化
	collider_->Initialize(context.objectCommon.debugObjectCommon, &object_->GetWorldTransform());

	//タグの設定
	collider_->SetTag(GetColliderTag());

	//大きさの設定
	collider_->SetRadius(GetColliderSize());

	/// === 他変数の設定 === ///

	//移動速度の設定
	speed_ = GetSpeed();

	//移動量の設定
	velocity_ = direction * speed_;

	//最大寿命の設定
	lifeTimeMax_ = GetLifeTime();

	isDead_ = false;
}

///=====================================================/// 
/// 弾の状態を更新
///=====================================================///
void BulletBase::Update() {

	//弾の寿命を更新
	lifeTimer_ += 1.0f / 60.0f;

	//弾の寿命が最大寿命を超えたら
	if (lifeTimer_ >= lifeTimeMax_) {

		isDead_ = true;
	}

	//弾の移動
	Move();

	//接触時判定
	IsCollision();

	//オブジェクトの更新
	object_->Update();

	//コライダーの更新
	collider_->Update();
}

///=====================================================/// 
/// 弾の座標更新処理を行う
///=====================================================///
void BulletBase::TransformUpdate() {

	//オブジェクトの座標のみ更新
	object_->Update();
}

///=====================================================/// 
/// 弾の描画処理を行う
///=====================================================///
void BulletBase::Draw() {

	//オブジェクトの描画
	object_->Draw(LayerType::OBJECT);

	//コライダーの描画
	collider_->Draw();
}

///=====================================================/// 
/// 弾の移動処理を行う
///=====================================================///
void BulletBase::Move() {

	//オブジェクトの移動
	object_->GetWorldTransform().translate_ += velocity_;
}
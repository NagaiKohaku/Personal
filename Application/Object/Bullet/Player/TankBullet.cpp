#include "TankBullet.h"

///=====================================================/// 
/// 弾が破棄される際に関連するコライダーを削除
///=====================================================///
TankBullet::~TankBullet() {

	//コライダーを削除
	collider_->Remove();
}

///=====================================================/// 
/// 弾を初期化し、パラメータを設定
///=====================================================///
void TankBullet::Initialize(Vector3 pos, Vector3 direction) {

	/// === オブジェクトの生成 === ///

	//生成
	object_ = std::make_unique<Object3D>();

	//初期化
	object_->Initialize();

	//座標の設定
	object_->GetWorldTransform().translate_ = pos;

	//サイズの設定
	object_->GetWorldTransform().scale_ = { 0.5f,0.5f,0.5f };

	//モデルの設定
	object_->SetModel("Sphere");

	/// === コライダーの生成 === ///

	//生成
	collider_ = std::make_unique<SphereCollider>();

	//初期化
	collider_->Initialize(&object_->GetWorldTransform());

	//タグの設定
	collider_->SetTag(Collider::Tag::PLAYERBULLETMEDIUM);

	//大きさの設定
	collider_->SetRadius(1.0f);

	/// === 他変数の設定 === ///

	//移動速度の設定
	speed_ = 1.5f;

	//移動量の設定
	velocity_ = direction * speed_;

	//最大寿命の設定
	lifeTimeMax_ = 2.0f;

}

///=====================================================/// 
/// 弾の状態を更新
///=====================================================///
void TankBullet::Update() {

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
void TankBullet::TransformUpdate() {

	//オブジェクトの座標のみ更新
	object_->Update();
}

///=====================================================/// 
/// 弾の描画処理を行う
///=====================================================///
void TankBullet::Draw() {

	//オブジェクトの描画
	object_->Draw(LayerType::OBJECT);

	//コライダーの描画
	collider_->Draw();
}

///=====================================================/// 
/// 弾の移動処理を行う
///=====================================================///
void TankBullet::Move() {

	//オブジェクトの移動
	object_->GetWorldTransform().translate_ += velocity_;
}

///=====================================================/// 
/// 他オブジェクトと接触している場合の処理を行う
///=====================================================///
void TankBullet::IsCollision() {

	//接触状態であれば
	if (collider_->GetIsTrigger()) {

		//接触相手のタグがENEMYであれば
		if (collider_->CheckHitTag(Collider::Tag::ENEMY)) {

			isDead_ = true;
		}
	}
}
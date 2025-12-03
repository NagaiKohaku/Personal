#include "JetBullet.h"

///=====================================================/// 
/// 弾が破棄される際に関連するコライダーを削除
///=====================================================///
JetBullet::~JetBullet() {

	//コライダーを削除
	collider_->Remove();
}

///=====================================================/// 
/// 弾を初期化し、パラメータを設定
///=====================================================///
void JetBullet::Initialize(Vector3 pos, Vector3 direction) {

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
	collider_->SetTag(Collider::Tag::PLAYERBULLETLIGHT);

	//大きさの設定
	collider_->SetRadius(0.5f);

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
void JetBullet::Update() {

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
void JetBullet::TransformUpdate() {

	//オブジェクトの座標のみ更新
	object_->Update();
}

///=====================================================/// 
/// 弾の描画処理を行う
///=====================================================///
void JetBullet::Draw() {

	//オブジェクトの描画
	object_->Draw(LayerType::Object);

	//コライダーの描画
	collider_->Draw();
}

///=====================================================/// 
/// 弾の移動処理を行う
///=====================================================///
void JetBullet::Move() {

	//オブジェクトの移動
	object_->GetWorldTransform().translate_ += velocity_;
}

///=====================================================/// 
/// 他オブジェクトと接触している場合の処理を行う
///=====================================================///
void JetBullet::IsCollision() {

	//接触状態であれば
	if (collider_->GetIsTrigger()) {

		//接触相手のタグがENEMYであれば
		if (collider_->CheckHitTag(Collider::Tag::ENEMY)) {

			isDead_ = true;
		}
	}
}
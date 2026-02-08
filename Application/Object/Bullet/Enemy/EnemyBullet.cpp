#include "EnemyBullet.h"

///=====================================================/// 
/// 弾が破棄される際に関連するコライダーを削除
///=====================================================///
EnemyBullet::~EnemyBullet() {

	//コライダーを削除
	collider_->Remove();
}

///=====================================================/// 
/// 弾を初期化し、パラメータを設定
///=====================================================///
void EnemyBullet::Initialize(Vector3 pos, Vector3 direction) {

	/// === パラメータの設定 === ///

	//大きさの設定
	size_ = 0.5f;

	//移動速度の設定
	speed_ = 0.5f;

	//移動量の設定
	velocity_ = direction * speed_;

	//最大寿命の設定
	lifeTimeMax_ = 2.0f;

	/// === オブジェクトの生成 === ///

	//生成
	object_ = std::make_unique<Object3D>();

	//初期化
	object_->Initialize();

	//座標の設定
	object_->GetWorldTransform().translate_ = pos;

	//サイズの設定
	object_->GetWorldTransform().scale_ = { size_,size_,size_ };

	//モデルの設定
	object_->SetModel("Sphere");

	/// === コライダーの生成 === ///

	//生成
	collider_ = std::make_unique<SphereCollider>();

	//初期化
	collider_->Initialize(&object_->GetWorldTransform());

	//タグの設定
	collider_->SetTag(Collider::Tag::ENEMYBULLET);

	//大きさの設定
	collider_->SetRadius(size_);
}

///=====================================================/// 
/// 弾の状態を更新
///=====================================================///
void EnemyBullet::Update() {

	//弾の寿命を更新
	lifeTimer_ += 1.0f / 60.0f;

	//弾の寿命が最大寿命を超えたら
	if (lifeTimer_ >= lifeTimeMax_) {

		isDead_ = true;
	}

	//弾の移動
	Move();

	//接触時処理
	IsCollision();

	//オブジェクトの更新
	object_->Update();

	//コライダーの更新
	collider_->Update();
}

///=====================================================/// 
/// 弾の座標更新処理を行う
///=====================================================///
void EnemyBullet::TransformUpdate() {

	//オブジェクトの座標のみ更新
	object_->Update();
}

///=====================================================/// 
/// 弾の描画処理を行う
///=====================================================///
void EnemyBullet::Draw() {

	//オブジェクトの描画
	object_->Draw(LayerType::OBJECT);

	//コライダーの描画
	collider_->Draw();
}

///=====================================================/// 
/// 弾の移動処理を行う
///=====================================================///
void EnemyBullet::Move() {

	//オブジェクトの移動
	object_->GetWorldTransform().translate_ += velocity_;
}

///=====================================================/// 
/// 他オブジェクトと接触している場合の処理を行う
///=====================================================///
void EnemyBullet::IsCollision() {

	//接触状態であれば
	if (collider_->GetIsTrigger()) {

		//接触相手のタグがPLAYERであれば
		if (collider_->CheckHitTag(Collider::Tag::PLAYER)) {

			isDead_ = true;
		}
	}
}
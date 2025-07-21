#include "Enemy.h"

#include "Math/Easing.h"

///=====================================================/// 
/// 初期化
///=====================================================///
void Enemy::Initialize(Camera* ptr) {

	//カメラのポインタを取得
	camera_ = ptr;

	/// === オブジェクトの生成 === ///

	//生成
	object_ = std::make_unique<Object3D>();

	//初期化
	object_->Initialize();

	//座標設定
	object_->GetWorldTransform().translate_ = { 0.0f,1.0f,0.0f };

	//モデルの設定
	object_->SetModel("Cube");

	/// === コライダーの生成 === ///

	//生成
	collider_ = std::make_unique<SphereCollider>();

	//初期化
	collider_->Initialize(&object_->GetWorldTransform());

	//タグの設定
	collider_->SetTag(Collider::Tag::ENEMY);

	//大きさの設定
	collider_->SetRadius(1.5f);

	/// === エミッターの生成 === ///

	//死亡時エミッターの生成
	explosiveEmitter_ = std::make_unique<EmitterGroup>();

	//死亡時エミッターの初期化
	explosiveEmitter_->Initialize(camera_);

	//死亡時エミッターのエミッター情報読み込み
	explosiveEmitter_->LoadEmitter("BlockExplosive");

	/// === 他変数の初期化 === ///

	//行動状態はエントリー状態で初期化
	state_ = ENTRY;

	//アニメーションタイマーの初期化
	animTimer_ = 0.0f;

	//エントリー時アニメーション終了時間の初期化
	entryAnimMaxTime_ = 1.0f;

	//離脱時アニメーション終了時間の初期化
	exitAnimMaxTime_ = 2.0f;

	//死亡時アニメーション終了時間の初期化
	deadAnimMaxTime_ = 5.0f;

	//離脱座標の初期化
	exitPos_ = { 0.0f, 0.0f, 20.0f };
}

void Enemy::Initialize(Camera* ptr, ObjectData objectData) {

	//カメラのポインタを取得
	camera_ = ptr;

	/// === オブジェクトの生成 === ///

	//生成
	object_ = std::make_unique<Object3D>();

	//初期化
	object_->Initialize(objectData);

	/// === コライダーの生成 === ///

	//生成
	collider_ = std::make_unique<SphereCollider>();

	//初期化
	collider_->Initialize(&object_->GetWorldTransform());

	//タグの設定
	collider_->SetTag(Collider::Tag::ENEMY);

	//大きさの設定
	collider_->SetRadius(1.5f);

	/// === エミッターの生成 === ///

	//死亡時エミッターの生成
	explosiveEmitter_ = std::make_unique<EmitterGroup>();

	//死亡時エミッターの初期化
	explosiveEmitter_->Initialize(camera_);

	//死亡時エミッターのエミッター情報読み込み
	explosiveEmitter_->LoadEmitter("BlockExplosive");

	/// === 他変数の初期化 === ///

	//行動状態はエントリー状態で初期化
	state_ = ENTRY;

	//アニメーションタイマーの初期化
	animTimer_ = 0.0f;

	//エントリー時アニメーション終了時間の初期化
	entryAnimMaxTime_ = 1.0f;

	//離脱時アニメーション終了時間の初期化
	exitAnimMaxTime_ = 2.0f;

	//死亡時アニメーション終了時間の初期化
	deadAnimMaxTime_ = 5.0f;

	//離脱座標の初期化
	exitPos_ = { 0.0f, 0.0f, 20.0f };
}

///=====================================================/// 
/// 更新処理
///=====================================================///
void Enemy::Update() {

	//衝突判定
	IsCollision();

	//行動状態によって変化
	switch (state_) {
	case Enemy::ENTRY:

		//エントリー時の処理
		Entry();

		break;
	case Enemy::STANDBY:

		break;
	case Enemy::MOVE:

		//移動処理
		Move();

		break;
	case Enemy::EXIT:

		//離脱時の処理
		Exit();

		break;
	case Enemy::DEAD:

		//死亡時の処理
		Dead();

		break;
	}

	//オブジェクトの更新
	object_->Update();

	//コライダーの処理
	collider_->Update();
}

///=====================================================/// 
/// 描画処理
///=====================================================///
void Enemy::Draw() {

	//オブジェクトの描画
	object_->Draw(LayerType::Object);

	//コライダーの描画
	collider_->Draw();

	//死亡時エミッターの描画
	explosiveEmitter_->Draw();
}

///=====================================================/// 
/// 衝突時処理
///=====================================================///
void Enemy::IsCollision() {

	//接触状態であれば
	if (collider_->GetIsCollision()) {

		//接触相手のタグがPLAYERBULLETであれば
		if (collider_->GetHitTag() == Collider::Tag::PLAYERBULLET) {

			//死亡状態に変更
			state_ = DEAD;

			//コライダーを削除
			collider_->Remove();

			//死亡時エミッターを起動
			explosiveEmitter_->Emit();

			//アニメーションタイマーの初期化
			animTimer_ = 0.0f;
		}
	}
}

///=====================================================/// 
/// 死亡時処理
///=====================================================///
void Enemy::Dead() {

	//アニメーションタイマーを進ませる
	animTimer_ += 1.0f / 60.0f;

	//下を向く
	object_->GetWorldTransform().rotate_ = { 1.0f,0.0f,0.0f };

	//下に落ちていく
	object_->GetWorldTransform().translate_.y -= 0.1f;

	//エミッターを中心座標に移動させる
	explosiveEmitter_->GetWorldTransform().translate_ = object_->GetWorldTransform().translate_;

	//エミッターの更新処理
	explosiveEmitter_->Update();

	//アニメーションタイマーが終了時間に達したら
	if (animTimer_ >= deadAnimMaxTime_) {

		//削除可能にする
		canRemove_ = true;
	}
}

///=====================================================/// 
/// エントリー時処理
///=====================================================///
void Enemy::Entry() {

	//アニメーションタイマーを進ませる
	animTimer_ += 1.0f / 60.0f;

	//アニメーションタイマーが終了時間に達したら
	if (animTimer_ >= entryAnimMaxTime_) {

		//前進状態に変更
		state_ = MOVE;

		//アニメーションタイマーをリセット
		animTimer_ = 0.0f;

		return;
	}

	//タイマーの比率
	float animRatio = animTimer_ / entryAnimMaxTime_;

	//オブジェクトの座標
	Vector3 objectPos = object_->GetWorldTransform().translate_;

	//スタンバイ座標までイージングで移動
	objectPos = EaseOut(objectPos, standbyPos_, animRatio, 0.1f);

	//移動後座標を設定
	object_->GetWorldTransform().translate_ = objectPos;
}

void Enemy::Move() {

	if (object_->GetWorldTransform().translate_.z <= exitPos_.z) {

		state_ = EXIT;

		return;
	}

	object_->GetWorldTransform().translate_.z -= 0.1f;

}

void Enemy::Exit() {

	//アニメーションタイマーを進ませる
	animTimer_ += 1.0f / 60.0f;

	//アニメーションタイマーが終了時間に達したら
	if (animTimer_ >= exitAnimMaxTime_) {

		//削除可能にする
		canRemove_ = true;

		//コライダーを削除
		collider_->Remove();

		return;
	}

	//タイマーの比率
	float animRatio = animTimer_ / exitAnimMaxTime_;

	//オブジェクトの座標
	Vector3 objectPos = object_->GetWorldTransform().translate_;

	Vector3 exitDirection = Normalize(Vector3(standbyPos_.x, standbyPos_.y, 0.0f));

	//出現座標
	Vector3 exitPos = exitDirection * 24.0f + exitPos_;

	//スタンバイ座標までイージングで移動
	objectPos = EaseOut(objectPos, exitPos, animRatio, 0.1f);

	//移動後座標を設定
	object_->GetWorldTransform().translate_ = objectPos;
}
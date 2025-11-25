#include "Enemy.h"

#include "Bullet/BulletManager.h"
#include "Player/Player.h"
#include "ObjectManager.h"

#include "Shake/Shake.h"

#include "Math/Easing.h"
#include "Math/Random.h"

#include "algorithm"
#include "numbers"

///=====================================================/// 
/// 初期化
///=====================================================///
void Enemy::Initialize(Camera* cameraPtr, BulletManager* bulletPtr, Player* playerPtr, ObjectData objectData) {

	//カメラのポインタを取得
	camera_ = cameraPtr;

	//バレットマネージャーのポインタを取得
	bulletManager_ = bulletPtr;

	//プレイヤーのポインタを取得
	player_ = playerPtr;

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

	//クリア時爆発エミッターの生成
	clearExplosiveEmitter_ = std::make_unique<EmitterGroup>();

	//クリア時爆発エミッターの初期化
	clearExplosiveEmitter_->Initialize(camera_);

	//クリア時爆発エミッターのエミッター情報読み込み
	clearExplosiveEmitter_->LoadEmitter("ClearExplosive");

	destroyEmitter_ = std::make_unique<EmitterGroup>();

	destroyEmitter_->Initialize(camera_);

	destroyEmitter_->LoadEmitter("Destroy");

	damageEmitter_ = std::make_unique<EmitterGroup>();

	damageEmitter_->Initialize(camera_);

	damageEmitter_->LoadEmitter("Damage");

	shadow_ = std::make_unique<Shadow>();

	shadow_->Initialize();

	/// === 他変数の初期化 === ///

	//行動状態はエントリー状態で初期化
	state_ = ENTRY;

	//体力の初期化
	hp_ = 3;

	//アニメーションタイマーの初期化
	animTimer_ = 0.0f;

	attackTimer_ = 0.0f;

	blinkTimer_ = 0.0f;

	//被弾揺れタイマーの初期化
	shakeTimer_ = 0.5f;

	shakeMaxTime_ = 0.5f;

	shakeLength_ = 0.5f;

	//アニメーション終了時間の初期化
	entryAnimMaxTime_ = 1.0f;

	moveAnimMaxTime_ = 5.0f;

	attackAnimMaxTime_ = 2.0f;

	exitAnimMaxTime_ = 2.0f;

	deadAnimMaxTime_ = 5.0f;

	//点滅フラグの初期化
	isBlink_ = false;

	//攻撃間隔の初期化
	attackFrequency_ = 1.0f;

	//点滅間隔の初期化
	blinkFrequency_ = 2.0f;

	canRemove_ = false;

	isRemove_ = false;

	isInvisible_ = false;
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
	case Enemy::MOVE:

		//移動処理
		Move();

		break;
	case Enemy::ATTACK:

		//攻撃処理
		Attack();

		break;
	case Enemy::EXIT:

		//離脱処理
		Exit();

		break;
	case Enemy::DEAD:

		//死亡処理
		Dead();

		break;
	}

	//点滅処理
	Blink();

	//前フレームの座標を取得
	prePos_ = object_->GetWorldTransform().translate_;

	//目標座標に移動
	object_->GetWorldTransform().translate_ = Lerp(object_->GetWorldTransform().translate_, targetPos_, 0.1f);

	//目標角度に回転
	object_->GetWorldTransform().rotate_ = Lerp(object_->GetWorldTransform().rotate_, targetRot_, 0.1f);

	//オブジェクトの更新
	object_->Update();

	Vector3 objectPos = object_->GetWorldTransform().translate_;

	//エミッターを中心座標に移動させる
	explosiveEmitter_->GetWorldTransform().translate_ = objectPos;

	clearExplosiveEmitter_->GetWorldTransform().translate_ = objectPos;

	destroyEmitter_->GetWorldTransform().translate_ = objectPos;

	damageEmitter_->GetWorldTransform().translate_ = objectPos;

	//エミッターの更新処理
	explosiveEmitter_->Update();

	//クリア時爆発エミッターの更新処理
	clearExplosiveEmitter_->Update();

	destroyEmitter_->Update();

	damageEmitter_->Update();

	//コライダーの処理
	collider_->Update();

	shadow_->Update(object_->GetWorldTransform().translate_);

	//被弾揺れ処理
	Shake();
}

void Enemy::TransformUpdate() {

	//オブジェクトの更新
	object_->Update();

	shadow_->Update(object_->GetWorldTransform().translate_);
}

///=====================================================/// 
/// 描画処理
///=====================================================///
void Enemy::Draw() {

	if (!isInvisible_) {

		//オブジェクトの描画
		object_->Draw(LayerType::Object);
	}

	//コライダーの描画
	collider_->Draw();

	//死亡時エミッターの描画
	explosiveEmitter_->Draw();

	//クリア時爆発エミッターの描画
	clearExplosiveEmitter_->Draw();

	destroyEmitter_->Draw();

	damageEmitter_->Draw();

	shadow_->Draw();
}

void Enemy::EmitClearExplosive() {

	state_ = DEAD;

	//クリア時爆発エミッターを起動
	clearExplosiveEmitter_->Emit();

	//エミッターを中心座標に移動させる
	clearExplosiveEmitter_->GetWorldTransform().translate_ = object_->GetWorldTransform().translate_;
}

void Enemy::ClearUpdate() {

	//下方向に移動させる
	object_->GetWorldTransform().translate_ = object_->GetWorldTransform().translate_ + Vector3(RandomFloat(-0.2f, 0.2f), RandomFloat(-1.5f, 0.0f), 0.0f);

	//ランダムに回転させる
	object_->GetWorldTransform().rotate_ = object_->GetWorldTransform().rotate_ + Vector3(
		RandomFloat(-1.0f, 1.0f),
		RandomFloat(-1.0f, 1.0f),
		RandomFloat(-1.0f, 1.0f)
	);

	//オブジェクトの更新
	object_->Update();

	shadow_->Update(object_->GetWorldTransform().translate_);

	clearExplosiveEmitter_->Update();
}

///=====================================================/// 
/// 衝突時処理
///=====================================================///
void Enemy::IsCollision() {

	//接触状態であれば
	if (collider_->GetIsTrigger()) {

		//接触相手のタグが弱攻撃であれば
		if (collider_->CheckHitTag(Collider::Tag::PLAYERBULLETLIGHT)) {

			damageEmitter_->Emit();

			hp_ -= 1;

			//被弾揺れタイマーを初期化
			shakeTimer_ = 0.0f;
		}

		//接触相手のタグが中攻撃であれば
		if (collider_->CheckHitTag(Collider::Tag::PLAYERBULLETMEDIUM)) {

			damageEmitter_->Emit();

			hp_ -= 3;

			//被弾揺れタイマーを初期化
			shakeTimer_ = 0.0f;
		}

		if (hp_ <= 0) {

			if (state_ != DEAD) {

				hp_--;

				//死亡状態に変更
				state_ = DEAD;

				//コライダーを削除
				collider_->Remove();

				//死亡時エミッターを起動
				explosiveEmitter_->Emit();

				//アニメーションタイマーの初期化
				animTimer_ = 0.0f;

				//被弾揺れタイマーを初期化
				shakeTimer_ = 0.0f;

				ObjectManager::GetInstance()->AddKillCount();

			}
		}
	}
}

///=====================================================/// 
/// 死亡時処理
///=====================================================///
void Enemy::Dead() {

	//アニメーションタイマーを進ませる
	animTimer_ += 1.0f / 60.0f;

	//アニメーションタイマーが終了時間に達したら
	if (animTimer_ >= deadAnimMaxTime_) {

		//削除可能にする
		canRemove_ = true;
	}

	//タイマーの比率
	float animRatio = animTimer_ / deadAnimMaxTime_;

	Vector3 objectPos = object_->GetWorldTransform().translate_;

	//下方向に落ちていくように設定
	targetPos_ = Lerp(objectPos, objectPos + Vector3(0.0f, -5.0f, -10.0f), animRatio);

	//Z軸で回転するように設定
	targetRot_ = Lerp(
		Vector3(0.0f, -std::numbers::pi_v<float>, 0.0f),
		Vector3(0.0f, -std::numbers::pi_v<float> *10.0f, std::numbers::pi_v<float> *10.0f),
		animRatio
	);

	if (object_->GetWorldTransform().translate_.y <= 0.0f) {

		if (!isInvisible_) {

			isInvisible_ = true;

			explosiveEmitter_->Stop();

			destroyEmitter_->Emit();

			Shake::GetInstance()->Start(0.5f, 0.5f);
		}
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

	//待機座標まで移動
	targetPos_ = EaseOutBack(entryPos_, standbyPos_, animRatio, 1.0f);

	//前方を向くように設定
	Vector3 velocity = prePos_ - targetPos_ + Vector3(0.0f, 0.0f, 10.0f);

	//移動方向を向くように設定
	targetRot_ = {
		Normalize(velocity).y,
		-std::numbers::pi_v<float>,
		Normalize(velocity).x
	};
}

///=====================================================/// 
/// 移動処理
///=====================================================///
void Enemy::Move() {

	//アニメーションタイマーを進ませる
	animTimer_ += 1.0f / 60.0f;

	//アニメーションタイマーが終了時間に達したら
	if (animTimer_ >= moveAnimMaxTime_) {

		//攻撃状態に設定
		state_ = ATTACK;

		//アニメーションタイマーをリセット
		animTimer_ = 0.0f;

		//攻撃状態になったら即攻撃できるように設定
		attackTimer_ = attackFrequency_;

		//点滅を終了
		isBlink_ = false;

		//オブジェクトの色を元に戻す
		object_->GetModel()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

		return;
	}

	//タイマーの比率
	float animRatio = animTimer_ / moveAnimMaxTime_;

	//離脱開始座標まで移動
	targetPos_ = Lerp(standbyPos_, exitStartPos_, animRatio);

	//前方を向くように設定
	targetRot_ = Vector3(0.0f, -std::numbers::pi_v<float>, 0.0f);

	//終了2秒前になったら
	if (animTimer_ >= moveAnimMaxTime_ - 2.0f) {

		//点滅を開始する
		isBlink_ = true;
	}
}

///=====================================================/// 
/// 攻撃処理
///=====================================================///
void Enemy::Attack() {

	//アニメーションタイマーを進ませる
	animTimer_ += 1.0f / 60.0f;

	//攻撃タイマーを進ませる
	attackTimer_ += 1.0f / 60.0f;

	//アニメーションタイマーが終了時間に達したら
	if (animTimer_ >= attackAnimMaxTime_) {

		//離脱状態に設定
		state_ = EXIT;

		//アニメーションタイマーをリセット
		animTimer_ = 0.0f;

		return;
	}

	//タイマーの比率
	float animRatio = animTimer_ / attackAnimMaxTime_;

	//攻撃可能になったら
	if (attackTimer_ >= attackFrequency_) {

		//プレイヤーへの方向
		Vector3 direction = player_->GetWorldPos() - object_->GetWorldTransform().translate_;

		//プレイヤーに向かって弾を発射
		bulletManager_->AddBullet(
			object_->GetWorldTransform().translate_,
			Normalize(direction),
			BulletManager::BULLETTYPE::ENEMY
		);

		//攻撃タイマーのリセット
		attackTimer_ = 0.0f;
	}
}

///=====================================================/// 
/// 離脱処理
///=====================================================///
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

	//離脱座標まで移動
	targetPos_ = EaseOut(exitStartPos_, exitPos_, animRatio, 2.0f);

	//前方を向くように設定
	Vector3 velocity = prePos_ - targetPos_ + Vector3(0.0f, 0.0f, 10.0f);

	//移動方向を向くように設定
	targetRot_ = {
		Normalize(velocity).y,
		-std::numbers::pi_v<float>,
		Normalize(velocity).x
	};
}

///=====================================================/// 
/// 点滅処理
///=====================================================///
void Enemy::Blink() {

	//フラグがfalseであれば
	if (!isBlink_) {

		//早期リターン
		return;
	}

	//タイマーを進ませる
	blinkTimer_ += 1.0f / 60.0f;

	//0.0f~1.0fの間を往復
	float blink = (sinf(blinkTimer_ * 2.0f * std::numbers::pi_v<float> *blinkFrequency_) + 1.0f) * 0.5f;

	//オブジェクトの色情報
	Vector4 color = object_->GetModel()->GetColor();

	//赤色に点滅するように設定
	color.y = 1.0f - blink;
	color.z = 1.0f - blink;

	//色情報を設定
	object_->GetModel()->SetColor(color);
}

void Enemy::Shake() {

	if (shakeTimer_ < shakeMaxTime_) {

		shakeTimer_ += 1.0f / 60.0f;
	} else {

		return;
	}

	float t = shakeTimer_ / shakeMaxTime_;

	float shakeLength = Lerp(shakeLength_, 0.0f, t);

	WorldTransform worldTransform = object_->GetWorldTransform();

	Vector3 randomOffset = {
		RandomFloat(-shakeLength, shakeLength),
		RandomFloat(-shakeLength, shakeLength),
		RandomFloat(-shakeLength, shakeLength)
	};

	worldTransform.translate_ = worldTransform.translate_ + randomOffset;

	worldTransform.UpdateMatrix();

	object_->GetWorldTransform().SetWorldMatrix(worldTransform.GetWorldMatrix());

	object_->TransformUpdate();
}
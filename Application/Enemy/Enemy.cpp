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
/// 敵オブジェクトを初期化し、必要なパラメータや関連リソースを設定
///=====================================================///
void Enemy::Initialize(Camera* cameraPtr, BulletManager* bulletPtr, Player* playerPtr, ObjectData objectData) {

	//カメラのポインタを取得
	camera_ = cameraPtr;

	//バレットマネージャーのポインタを取得
	bulletManager_ = bulletPtr;

	//プレイヤーのポインタを取得
	player_ = playerPtr;

	/// === オブジェクトの生成 === ///

	//敵オブジェクトの生成と初期化
	object_ = std::make_unique<Object3D>();
	object_->Initialize(objectData);

	//影オブジェクトの生成と初期化
	shadow_ = std::make_unique<Shadow>();
	shadow_->Initialize();

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

	//死亡時エミッターの生成とエミッター情報読み込み
	explosiveEmitter_ = std::make_unique<EmitterGroup>();
	explosiveEmitter_->Initialize(camera_);
	explosiveEmitter_->LoadEmitter("BlockExplosive");

	//クリア時爆発エミッターの生成とエミッター情報読み込み
	clearExplosiveEmitter_ = std::make_unique<EmitterGroup>();
	clearExplosiveEmitter_->Initialize(camera_);
	clearExplosiveEmitter_->LoadEmitter("ClearExplosive");

	//破壊時エミッターの生成とエミッター情報読み込み
	destroyEmitter_ = std::make_unique<EmitterGroup>();
	destroyEmitter_->Initialize(camera_);
	destroyEmitter_->LoadEmitter("Destroy");

	//ダメージ時エミッターの生成とエミッター情報読み込み
	damageEmitter_ = std::make_unique<EmitterGroup>();
	damageEmitter_->Initialize(camera_);
	damageEmitter_->LoadEmitter("Damage");

	/// === 他変数の初期化 === ///

	//行動状態はエントリー状態で初期化
	state_ = ENTRY;

	//体力の初期化
	hp_ = 3;

	//弱攻撃ヒット時のダメージの初期化
	lightAttackDamage_ = 1;

	//中攻撃ヒット時のダメージの初期化
	mediumAttackDamage_ = 3;

	//移動強度の初期化
	moveStrength_ = 10.0f;

	//死亡時移動量の設定
	deadMoveVelocity_ = Vector3(0.0f, -5.0f, -10.0f);

	// 攻撃系の初期化 //

	//攻撃タイマーの初期化
	attackTimer_ = 0.0f;

	//攻撃間隔の初期化
	attackFrequency_ = 1.0f;

	// 点滅系の初期化 //

	//点滅タイマーの初期化
	blinkTimer_ = 0.0f;

	//点滅間隔の初期化
	blinkFrequency_ = 2.0f;

	//点滅フラグの初期化
	isBlink_ = false;

	// シェイク系の初期化 //

	//タイマーの初期化
	shakeTimer_ = 0.5f;

	//最大時間の初期化
	shakeMaxTime_ = 0.5f;

	//揺れの長さを初期化
	shakeLength_ = 0.5f;

	//画面揺れの時間を初期化
	displayShakeTime_ = 0.5f;

	//画面揺れの長さを初期化
	displayShakeLength_ = 0.5f;

	// アニメーション系の初期化 //

	//アニメーションタイマーの初期化
	animTimer_ = 0.0f;

	//アニメーション終了時間の初期化
	entryAnimMaxTime_ = 1.0f;

	moveAnimMaxTime_ = 5.0f;

	attackAnimMaxTime_ = 2.0f;

	exitAnimMaxTime_ = 2.0f;

	deadAnimMaxTime_ = 5.0f;

	//削除可能フラグの初期化
	canRemove_ = false;

	//削除中フラグの初期化
	isRemove_ = false;

	//透明化フラグの初期化
	isInvisible_ = false;
}

///=====================================================/// 
/// 敵オブジェクトの状態を毎フレーム更新
///=====================================================///
void Enemy::Update() {

	/// === 敵オブジェクトの更新 === ///

	//前フレームの座標を取得
	prePos_ = object_->GetWorldTransform().translate_;

	//死亡状態の確認
	CheckDead();

	//衝突時処理
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

	//被弾揺れ処理
	Shake();

	//点滅処理
	Blink();

	//コライダーの処理
	collider_->Update();

	//影オブジェクトの更新
	shadow_->Update(object_->GetWorldTransform().translate_);

	//目標座標に移動
	object_->GetWorldTransform().translate_ = Lerp(object_->GetWorldTransform().translate_, targetPos_, moveStrength_ / 100.0f);

	//目標角度に回転
	object_->GetWorldTransform().rotate_ = Lerp(object_->GetWorldTransform().rotate_, targetRot_, moveStrength_ / 100.0f);

	//オブジェクトの更新
	object_->Update();

	/// === エミッターの更新 === ///

	//敵オブジェクトの中心座標
	Vector3 objectPos = object_->GetWorldTransform().translate_;

	//各エミッターを中心座標に移動させる
	explosiveEmitter_->GetWorldTransform().translate_ = objectPos;
	clearExplosiveEmitter_->GetWorldTransform().translate_ = objectPos;
	destroyEmitter_->GetWorldTransform().translate_ = objectPos;
	damageEmitter_->GetWorldTransform().translate_ = objectPos;

	//各エミッターの更新処理
	explosiveEmitter_->Update();
	clearExplosiveEmitter_->Update();
	destroyEmitter_->Update();
	damageEmitter_->Update();
}

///=====================================================/// 
/// 敵オブジェクトと影オブジェクトの座標情報のみを更新
///=====================================================///
void Enemy::TransformUpdate() {

	//敵オブジェクトの更新
	object_->Update();

	//影オブジェクトの更新
	shadow_->Update(object_->GetWorldTransform().translate_);
}

///=====================================================/// 
/// クリア時の敵オブジェクトの移動・回転処理を更新
///=====================================================///
void Enemy::ClearUpdate() {

	//下方向に移動させる
	object_->GetWorldTransform().translate_ = object_->GetWorldTransform().translate_ + deadMoveVelocity_;

	//ランダムに回転させる
	object_->GetWorldTransform().rotate_ = object_->GetWorldTransform().rotate_ + Vector3(
		RandomFloat(-std::numbers::pi_v<float>, std::numbers::pi_v<float>),
		RandomFloat(-std::numbers::pi_v<float>, std::numbers::pi_v<float>),
		RandomFloat(-std::numbers::pi_v<float>, std::numbers::pi_v<float>)
	);

	//敵オブジェクトの更新
	object_->Update();

	//影オブジェクトの更新
	shadow_->Update(object_->GetWorldTransform().translate_);

	//クリア時爆発エミッターの更新
	clearExplosiveEmitter_->Update();
}

///=====================================================/// 
/// 敵オブジェクトおよび関連する要素の描画処理
///=====================================================///
void Enemy::Draw() {

	//透明化中は描画しない
	if (!isInvisible_) {

		//オブジェクトの描画
		object_->Draw(LayerType::Object);
	}

	//影オブジェクトの描画
	shadow_->Draw();

	//コライダーの描画
	collider_->Draw();

	//各エミッターの描画
	explosiveEmitter_->Draw();
	clearExplosiveEmitter_->Draw();
	destroyEmitter_->Draw();
	damageEmitter_->Draw();
}

///=====================================================/// 
/// 敵のクリア時爆発エミッターを起動
///=====================================================///
void Enemy::EmitClearExplosive() {

	//死亡状態に移行
	state_ = DEAD;

	//クリア時爆発エミッターを起動
	clearExplosiveEmitter_->Emit();
}

///=====================================================/// 
/// 他オブジェクトの接触時の処理
///=====================================================///
void Enemy::IsCollision() {

	//コライダーがなければスキップ
	if (!collider_) {
		return;
	}

	//接触状態であれば
	if (collider_->GetIsTrigger()) {

		//接触相手のタグが弱攻撃であれば
		if (collider_->CheckHitTag(Collider::Tag::PLAYERBULLETLIGHT)) {

			damageEmitter_->Emit();

			hp_ -= lightAttackDamage_;

			//被弾揺れタイマーを初期化
			shakeTimer_ = 0.0f;
		}

		//接触相手のタグが中攻撃であれば
		if (collider_->CheckHitTag(Collider::Tag::PLAYERBULLETMEDIUM)) {

			damageEmitter_->Emit();

			hp_ -= mediumAttackDamage_;

			//被弾揺れタイマーを初期化
			shakeTimer_ = 0.0f;
		}
	}
}

///=====================================================/// 
/// 敵オブジェクトのエントリー時の処理
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
	targetPos_ = EaseOutBack(entryPos_, standbyPos_, animRatio, 2.0f);

	//前フレームからの移動量を計算
	//前方を向くようにZ方向成分を強めに設定
	Vector3 velocity = prePos_ - targetPos_ + Vector3(0.0f, 0.0f, 10.0f);

	//移動方向を向くように設定
	targetRot_ = {
		Normalize(velocity).y,
		-std::numbers::pi_v<float>,
		Normalize(velocity).x
	};
}

///=====================================================/// 
/// 敵オブジェクトの移動時の処理
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
/// 敵オブジェクトの攻撃時の処理
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
/// 敵オブジェクトの離脱時の処理
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

	//前フレームからの移動量を計算
	//前方を向くようにZ方向成分を強めに設定
	Vector3 velocity = prePos_ - targetPos_ + Vector3(0.0f, 0.0f, 10.0f);

	//移動方向を向くように設定
	targetRot_ = {
		Normalize(velocity).y,
		-std::numbers::pi_v<float>,
		Normalize(velocity).x
	};
}

///=====================================================/// 
/// 敵オブジェクトの死亡時の処理
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
	targetPos_ = Lerp(objectPos, objectPos + deadMoveVelocity_, animRatio);

	//Y軸とZ軸で回転するように設定
	targetRot_ = Lerp(
		Vector3(0.0f, -std::numbers::pi_v<float>, 0.0f),
		Vector3(0.0f, -std::numbers::pi_v<float> * 10.0f, std::numbers::pi_v<float> * 10.0f),
		animRatio
	);

	//地面よりも下にいたら透明化させる
	if (object_->GetWorldTransform().translate_.y <= 0.0f) {

		if (!isInvisible_) {

			//透明化を有効化
			isInvisible_ = true;

			//爆発エミッターをストップ
			explosiveEmitter_->Stop();

			//破壊時エミッターを起動
			destroyEmitter_->Emit();

			//シェイクを開始
			Shake::GetInstance()->Start(displayShakeLength_, displayShakeTime_);
		}
	}
}

///=====================================================/// 
/// 敵オブジェクトの点滅処理
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

///=====================================================/// 
/// 敵オブジェクトの被弾時のシェイク処理
///=====================================================///
void Enemy::Shake() {

	//タイマーが最大時間以下であれば進ませる
	if (shakeTimer_ < shakeMaxTime_) {

		shakeTimer_ += 1.0f / 60.0f;
	} else {

		return;
	}

	//タイマーの比率
	float t = shakeTimer_ / shakeMaxTime_;

	//シェイクの長さ
	float shakeLength = Lerp(shakeLength_, 0.0f, t);

	//オブジェクトの座標をコピー
	WorldTransform worldTransform = object_->GetWorldTransform();

	//シェイクのオフセット
	Vector3 shakeOffset = {
		RandomFloat(-shakeLength, shakeLength),
		RandomFloat(-shakeLength, shakeLength),
		RandomFloat(-shakeLength, shakeLength)
	};

	//オブジェクトの座標にシェイクの座標を加える
	worldTransform.translate_ = worldTransform.translate_ + shakeOffset;

	//シェイク後の座標で更新
	worldTransform.UpdateMatrix();

	//座標変換データのみシェイク後の座標に変更
	object_->GetWorldTransform().SetWorldMatrix(worldTransform.GetWorldMatrix());

	//オブジェクトの座標データのみを更新
	object_->TransformUpdate();
}

///=====================================================/// 
/// 敵のHPを確認し、死亡状態に遷移させる処理
///=====================================================///
void Enemy::CheckDead() {

	//HPがなくなっていたら
	if (hp_ <= 0) {

		//すでに死亡状態であればスキップ
		if (state_ != DEAD) {

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

			//キル数をカウントアップ
			ObjectManager::GetInstance()->AddKillCount();
		}
	}
}
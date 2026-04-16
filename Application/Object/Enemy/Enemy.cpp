#include "Enemy.h"

#include <Object/Bullet/Core/BulletManager.h>
#include <Object/Player/Player.h>
#include <Object/Manager/ObjectManager.h>

#include <Effect/Shake/Shake.h>

#include <Math/Utility/Easing.h>
#include <Math/Utility/Random.h>

#include "algorithm"
#include "numbers"

using namespace MyEngine;

///=====================================================/// 
/// 敵オブジェクトを初期化し、必要なパラメータや関連リソースを設定
///=====================================================///
void Enemy::Initialize(EngineContext context, BulletManager* bulletPtr, Player* playerPtr, ObjectData objectData) {

	//カメラのポインタを取得
	camera_ = context.camera;

	//バレットマネージャーのポインタを取得
	bulletManager_ = bulletPtr;

	//プレイヤーのポインタを取得
	player_ = playerPtr;

	/// === オブジェクトの生成 === ///

	//敵オブジェクトの生成と初期化
	object_ = context.objectManager->CreateObject3D();
	object_->SetObjectData(objectData);

	//影オブジェクトの生成と初期化
	shadow_ = std::make_unique<Shadow>();
	shadow_->Initialize(context);

	/// === コライダーの生成 === ///

	//生成
	collider_ = std::make_unique<SphereCollider>();

	//初期化
	collider_->Initialize(context.objectCommon.debugObjectCommon, context.renderer, &object_->GetWorldTransform());

	//タグの設定
	collider_->SetTag(Collider::Tag::ENEMY);

	//大きさの設定
	collider_->SetRadius(1.5f);

	/// === エミッターの生成 === ///

	//死亡時エミッターの生成とエミッター情報読み込み
	explosiveEmitter_ = std::make_unique<EmitterGroup>();
	explosiveEmitter_->Initialize(context.objectCommon.particleCommon, camera_, context.input, context.renderer);
	explosiveEmitter_->LoadEmitter("BlockExplosive");

	//クリア時爆発エミッターの生成とエミッター情報読み込み
	clearExplosiveEmitter_ = std::make_unique<EmitterGroup>();
	clearExplosiveEmitter_->Initialize(context.objectCommon.particleCommon, camera_, context.input, context.renderer);
	clearExplosiveEmitter_->LoadEmitter("ClearExplosive");

	//破壊時エミッターの生成とエミッター情報読み込み
	destroyEmitter_ = std::make_unique<EmitterGroup>();
	destroyEmitter_->Initialize(context.objectCommon.particleCommon, camera_, context.input, context.renderer);
	destroyEmitter_->LoadEmitter("Destroy");

	//ダメージ時エミッターの生成とエミッター情報読み込み
	damageEmitter_ = std::make_unique<EmitterGroup>();
	damageEmitter_->Initialize(context.objectCommon.particleCommon, camera_, context.input, context.renderer);
	damageEmitter_->LoadEmitter("Damage");

	/// === 他変数の初期化 === ///

	//行動状態はエントリー状態で初期化
	state_ = std::make_unique<EnemyEntryState>();
	state_->Enter();

	//体力の初期化
	hp_ = 3;

	//死亡時のHPの初期化
	deadHp_ = 100;

	//弱攻撃ヒット時のダメージの初期化
	lightAttackDamage_ = 1;

	//中攻撃ヒット時のダメージの初期化
	mediumAttackDamage_ = 3;

	//死亡時移動量の設定
	deadMoveVelocity_ = Vector3(0.0f, -5.0f, -10.0f);

	// 点滅系の初期化 //

	//点滅タイマーの初期化
	blinkTimer_ = 0.0f;

	//点滅間隔の初期化
	blinkFrequency_ = 2.0f;

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

	//削除可能フラグの初期化
	canRemove_ = false;

	//削除中フラグの初期化
	isRemove_ = false;

	//透明化フラグの初期化
	isInvisible_ = false;

	isBlink_ = false;
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

	state_->Update(*this);

	//衝突時処理
	IsCollision();

	//被弾揺れ処理
	Shake();

	//点滅処理
	Blink();

	//コライダーの処理
	collider_->Update();

	//影オブジェクトの更新
	shadow_->Update(object_->GetWorldTransform().translate_);

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
		object_->Draw(LayerType::OBJECT);
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

	//クリア時爆発エミッターを起動
	clearExplosiveEmitter_->Emit();
}

void Enemy::ChangeState(std::unique_ptr<EnemyState> state) {

	state_ = std::move(state);
	state_->Enter();
}

void Enemy::StartBlink() {

	isBlink_ = true;

	blinkTimer_ = 0.0f;
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

	object_->GetModel()->GetModelParts()[0].material->color.y = 1.0f - blink;
	object_->GetModel()->GetModelParts()[0].material->color.z = 1.0f - blink;
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
}

///=====================================================/// 
/// 敵のHPを確認し、死亡状態に遷移させる処理
///=====================================================///
void Enemy::CheckDead() {

	//HPがなくなっていたら
	if (hp_ <= 0) {

		//二度とここに来ないように大きな値を入れておく
		hp_ = deadHp_;

		//状態を死亡状態に変更
		ChangeState(std::make_unique<EnemyDeadState>());

		//コライダーを削除
		collider_->Remove();

		//死亡時エミッターを起動
		explosiveEmitter_->Emit();

		//被弾揺れタイマーを初期化
		shakeTimer_ = 0.0f;

		//Todo:コメントアウト

		////キル数をカウントアップ
		//ObjectManager::GetInstance()->AddKillCount();

	}
}

void Enemy::StartRemove() {

	//削除可能にする
	canRemove_ = true;

	//コライダーを削除
	collider_->Remove();

	object_->SetIsRemove(true);

}

void Enemy::StartDestroy() {

	//透明化を有効化
	isInvisible_ = true;

	//爆発エミッターをストップ
	explosiveEmitter_->Stop();

	//破壊時エミッターを起動
	destroyEmitter_->Emit();

	//シェイクを開始
	Shake::GetInstance()->Start(displayShakeLength_, displayShakeTime_);
}

void Enemy::ResetColor() {

	//オブジェクトの色を元に戻す
	object_->GetModel()->GetModelParts()[0].material->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	isBlink_ = false;
}

Vector3 Enemy::GetPlayerPos() {
	return player_->GetWorldPos();
}

bool Enemy::GetIsDead() {

	if (auto* startEvent = dynamic_cast<EnemyDeadState*>(state_.get())) {

		return true;
	}

	return false;
}

BulletManager* Enemy::GetBulletManager() {
	return bulletManager_;
}
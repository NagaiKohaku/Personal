#pragma once

#include "3d/Object/Object3D.h"
#include "3d/Particle/EmitterGroup.h"
#include "3d/Camera/Camera.h"

#include "Shadow/Shadow.h"

#include "3d/Collider/AABBCollider.h"
#include "3d/Collider/SphereCollider.h"

#include "memory"

/// === 前方宣言 === ///

class BulletManager;

class Player;

/// <summary>
/// 敵キャラクターを管理するクラスです。
/// </summary>
/// <remarks>
/// - 敵の行動状態（ENTRY, MOVE, ATTACK, EXIT, DEAD）を管理します。  
/// - 敵オブジェクトや影オブジェクト、当たり判定用コライダーを保持します。  
/// - 死亡、クリア、破壊、ダメージ用の各種エミッターを管理します。  
/// - 移動座標、回転角度、移動強度などの物理情報を保持します。  
/// - 体力や攻撃力、攻撃間隔など戦闘に関する情報を保持します。  
/// - 点滅や被弾揺れ、画面揺れなどの演出に関する情報を保持します。  
/// - 各種アニメーションタイマーや最大時間、削除フラグ、透明化フラグを管理します。  
/// - 敵の初期化、更新、描画、攻撃、移動、死亡処理などの一連の動作を提供します。  
/// </remarks>
class Enemy {

	///-------------------------------------------/// 
	/// 列挙型
	///-------------------------------------------///
private:

	//行動状態
	enum STATE {
		ENTRY,
		MOVE,
		ATTACK,
		EXIT,
		DEAD
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 敵オブジェクトを初期化し、必要なパラメータや関連リソースを設定します。
	/// </summary>
	/// <param name="cameraPtr">追従対象のカメラを指定します。</param>
	/// <param name="bulletPtr">弾管理クラスのポインタを指定します。</param>
	/// <param name="playerPtr">プレイヤーオブジェクトのポインタを指定します。</param>
	/// <param name="objectData">敵オブジェクト生成に必要なデータを指定します。</param>
	/// <remarks>
	/// - カメラ、弾管理、プレイヤーのポインタを保持します。  
	/// - 敵オブジェクトおよび影オブジェクトを生成し初期化します。  
	/// - 当たり判定用の球体コライダーを生成し、タグや大きさを設定します。  
	/// - 死亡、クリア、破壊、ダメージ用のエミッターを生成し、設定ファイルから情報を読み込みます。  
	/// - 行動状態や体力、被ダメージ量、移動強度など各種変数を初期化します。  
	/// - 点滅や画面揺れ、アニメーションのタイマーや最大時間を初期化します。  
	/// - 削除可能フラグや透明化フラグなどの状態フラグを初期化します。  
	/// </remarks>
	void Initialize(Camera* cameraPtr, BulletManager* bulletPtr, Player* playerPtr, ObjectData objectData);

	/// <summary>
	/// 敵オブジェクトの状態を毎フレーム更新します。
	/// </summary>
	/// <remarks>
	/// - 前フレームの座標を保存します。  
	/// - 死亡状態や衝突判定を確認します。  
	/// - 現在の行動状態に応じて、エントリー、移動、攻撃、離脱、死亡処理を実行します。  
	/// - 被弾による揺れや点滅処理を行います。  
	/// - コライダーと影オブジェクトを更新します。  
	/// - 目標座標や角度に向かってオブジェクトを滑らかに移動・回転させます。  
	/// - 各種エミッターを敵の中心座標に同期させ、更新処理を実行します。  
	/// </remarks>
	void Update();

	/// <summary>
	/// 敵オブジェクトと影オブジェクトの座標情報のみを更新します。
	/// </summary>
	/// <remarks>
	/// - 表示用オブジェクトの更新処理を行います。  
	/// - 影オブジェクトの座標を敵オブジェクトの現在座標に同期します。  
	/// </remarks>
	void TransformUpdate();

	/// <summary>
	/// クリア時の敵オブジェクトの移動・回転処理を更新します。
	/// </summary>
	/// <remarks>
	/// - オブジェクトを下方向に移動させます。  
	/// - オブジェクトをランダムに回転させます。  
	/// - 敵オブジェクトと影オブジェクトを更新します。  
	/// - クリア時用の爆発エミッターを更新します。  
	/// </remarks>
	void ClearUpdate();

	/// <summary>
	/// 敵オブジェクトおよび関連する要素の描画処理を行います。
	/// </summary>
	/// <remarks>
	/// - 透明化中でなければ敵オブジェクトを描画します。  
	/// - 影オブジェクトを描画します。  
	/// - 当たり判定用コライダーを描画します。  
	/// - 死亡、クリア、破壊、ダメージ用の各エミッターを描画します。  
	/// </remarks>
	void Draw();

	/// <summary>
	/// 敵のクリア時爆発エミッターを起動します。
	/// </summary>
	/// <remarks>
	/// - 敵の状態を死亡状態に設定します。  
	/// - クリア時用の爆発エミッターを発生させます。  
	/// </remarks>
	void EmitClearExplosive();

	/// <summary>
	/// 死亡状態のチェック
	/// </summary>
	/// <returns>死亡状態の場合は true : 違う場合は false</returns>
	bool CheckIsDead() { return state_ == DEAD; }

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 他オブジェクトの接触時の処理を行います。
	/// </summary>
	/// <remarks>
	/// - コライダーが存在しない場合は処理を行いません。  
	/// - 当たり判定用コライダーが接触状態であるかを確認します。  
	/// - 接触相手がプレイヤーの攻撃であれば、ダメージエミッターを発生させ、体力を減少させます。  
	/// - 被弾による揺れタイマーをリセットします。  
	/// </remarks>
	void IsCollision();

	/// <summary>
	/// 敵オブジェクトのエントリー時の処理を行います。
	/// </summary>
	/// <remarks>
	/// - アニメーションタイマーを進め、終了時間に達したら状態を移動（MOVE）に変更します。  
	/// - タイマーの進行比率に基づき、出現座標から待機座標まで滑らかに移動させます。  
	/// - 前フレームとの座標差から移動方向を計算し、前方を向くように回転角度を設定します。  
	/// </remarks>
	void Entry();

	/// <summary>
	/// 敵オブジェクトの移動時の処理を行います。
	/// </summary>
	/// <remarks>
	/// - アニメーションタイマーを進め、終了時間に達したら攻撃状態（ATTACK）に移行します。  
	/// - タイマー比率に応じて待機座標から離脱開始座標まで滑らかに移動させます。  
	/// - 前方を向くように回転角度を設定します。  
	/// - 終了2秒前から点滅を開始します。  
	/// </remarks>
	void Move();

	/// <summary>
	/// 敵オブジェクトの攻撃時の処理を行います。
	/// </summary>
	/// <remarks>
	/// - アニメーションタイマーと攻撃タイマーを進めます。  
	/// - アニメーションタイマーが終了時間に達したら離脱状態（EXIT）に移行します。  
	/// - 攻撃タイマーが攻撃間隔に達した場合、プレイヤーの方向を計算し、敵弾を発射します。  
	/// - 攻撃後は攻撃タイマーをリセットして次の攻撃準備を行います。  
	/// </remarks>
	void Attack();

	/// <summary>
	/// 敵オブジェクトの離脱時の処理を行います。
	/// </summary>
	/// <remarks>
	/// - アニメーションタイマーを進め、終了時間に達したら削除可能フラグを立て、コライダーを削除します。  
	/// - タイマー比率に基づき、離脱開始座標から離脱終了座標まで滑らかに移動させます。  
	/// - 前フレームからの移動量を計算し、前方を向くように回転角度を設定します。  
	/// </remarks>
	void Exit();

	/// <summary>
	/// 敵オブジェクトの死亡時の処理を行います。
	/// </summary>
	/// <remarks>
	/// - アニメーションタイマーを進め、終了時間に達したら削除可能フラグを立てます。  
	/// - タイマー比率に基づき、下方向への移動とY/Z軸での回転を滑らかに行います。  
	/// - オブジェクトが地面より下に到達した場合、透明化を有効化、破壊時エミッターと画面シェイクを開始します。  
	/// </remarks>
	void Dead();

	/// <summary>
	/// 敵オブジェクトの点滅処理を行います。
	/// </summary>
	/// <remarks>
	/// - 点滅フラグが有効な場合のみ処理を行います。  
	/// - 点滅タイマーを進め、0.0f～1.0fの範囲で往復する値を計算します。  
	/// - 計算した値に応じてオブジェクトの赤色以外のチャンネルを減少させ、赤色に点滅するように設定します。  
	/// - 設定後の色情報をオブジェクトに反映させます。  
	/// </remarks>
	void Blink();

	/// <summary>
	/// 敵オブジェクトの被弾時のシェイク処理を行います。
	/// </summary>
	/// <remarks>
	/// - シェイクタイマーが最大時間以下の場合のみ処理を行い、タイマーを進めます。  
	/// - タイマー比率に応じてシェイクの長さを減衰させます。  
	/// - オブジェクトの座標にランダムなオフセットを加え、揺れを表現します。  
	/// - 座標変換データを更新し、オブジェクトの表示に反映させます。  
	/// </remarks>
	void Shake();

	/// <summary>
	/// 敵のHPを確認し、死亡状態に遷移させる処理を行います。
	/// </summary>
	/// <remarks>
	/// - HPが0以下の場合にのみ処理を行います。  
	/// - すでに死亡状態であれば処理を行いません。  
	/// - 死亡状態に変更し、コライダーを削除します。  
	/// - 死亡時エミッターを発生させ、アニメーションタイマーとシェイクタイマーを初期化します。  
	/// - キル数をカウントアップします。  
	/// </remarks>
	void CheckDead();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	Camera* camera_;

	//バレットマネージャー
	BulletManager* bulletManager_;

	//プレイヤー
	Player* player_;

	//オブジェクト
	std::unique_ptr<Object3D> object_ = nullptr;

	//コライダー
	std::unique_ptr<SphereCollider> collider_ = nullptr;

	//死亡時エミッター
	std::unique_ptr<EmitterGroup> explosiveEmitter_;

	//クリア時爆発エミッター
	std::unique_ptr<EmitterGroup> clearExplosiveEmitter_;

	//破壊時エミッター
	std::unique_ptr<EmitterGroup> destroyEmitter_;

	//ダメージ時エミッター
	std::unique_ptr<EmitterGroup> damageEmitter_;

	//影オブジェクト
	std::unique_ptr<Shadow> shadow_;

	//行動状態
	STATE state_;

	//前フレームの座標
	Vector3 prePos_;

	//移動目標座標
	Vector3 targetPos_;

	//回転目標角度
	Vector3 targetRot_;

	//移動強度(％)
	float moveStrength_;

	//エントリー状態の座標
	Vector3 entryPos_;

	//スタンバイ状態の座標
	Vector3 standbyPos_;

	//離脱開始座標
	Vector3 exitStartPos_;

	//離脱座標
	Vector3 exitPos_;

	//死亡時移動量
	Vector3 deadMoveVelocity_;

	//体力
	int hp_;

	//弱攻撃ヒット時のダメージ
	int lightAttackDamage_;

	//中攻撃ヒット時のダメージ
	int mediumAttackDamage_;

	//攻撃タイマー
	float attackTimer_;

	//攻撃間隔
	float attackFrequency_;

	//点滅タイマー
	float blinkTimer_;

	//点滅間隔
	float blinkFrequency_;

	//点滅フラグ
	bool isBlink_;

	//被弾揺れタイマー
	float shakeTimer_;

	//被弾揺れ最大時間
	float shakeMaxTime_;

	//被弾揺れの長さ
	float shakeLength_;

	//画面揺れの時間
	float displayShakeTime_;

	//画面揺れの長さ
	float displayShakeLength_;

	//アニメーションタイマー
	float animTimer_;

	//エントリー時アニメーション終了時間
	float entryAnimMaxTime_;

	//移動時のアニメーション終了時間
	float moveAnimMaxTime_;

	//攻撃時のアニメーション終了時間
	float attackAnimMaxTime_;

	//離脱時アニメーション終了時間
	float exitAnimMaxTime_;

	//死亡時アニメーション終了時間
	float deadAnimMaxTime_;

	//削除可能フラグ
	bool canRemove_;

	//削除中フラグ
	bool isRemove_;

	//透明化フラグ
	bool isInvisible_;

	///-------------------------------------------///
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// 座標のセッター
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(Vector3 pos) { object_->GetWorldTransform().translate_ = pos; }

	/// <summary>
	/// エントリーの位置を設定します。
	/// </summary>
	/// <param name="pos">座標</param>
	void SetEntryPos(Vector3 pos) { entryPos_ = pos; }

	/// <summary>
	/// スタンバイ状態の座標のセッター
	/// </summary>
	/// <param name="pos">座標</param>
	void SetStandbyPos(Vector3 pos) { standbyPos_ = pos; }

	/// <summary>
	/// 離脱開始座標の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetExitStartPos(Vector3 pos) { exitStartPos_ = pos; }

	/// <summary>
	/// 離脱状態の座標の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetExitPos(Vector3 pos) { exitPos_ = pos; }

	/// <summary>
	/// 削除中フラグのセッター
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetIsRemove(bool flag) { isRemove_ = flag; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPos() { return object_->GetWorldTransform().GetWorldTranslate(); }

	/// <summary>
	/// 削除可能フラグのゲッター
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetCanRemove() { return canRemove_; }

	/// <summary>
	/// 削除中フラグのゲッター
	/// </summary>
	/// <returns>フラグ</returns>
	bool GetIsRemove() { return isRemove_; }

};
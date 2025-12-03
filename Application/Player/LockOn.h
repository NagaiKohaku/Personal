#pragma once

#include "Reticle.h"

#include "memory"
#include "vector"
#include "list"

class Player;
class Enemy;
class Camera;

/// <summary>
/// ロックオンシステムを管理するクラスです。
/// </summary>
/// <remarks>
/// - プレイヤーの周囲にメインレティクルとロックオンレティクルを表示し、敵を自動でロックオンします。
/// - ロックオン対象の敵情報を管理し、レティクルの更新・描画を行います。
/// - 最大ロックオン数に応じて複数の敵を同時に追尾可能です。
/// </remarks>
class LockOn {

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
public:

	/// ロックオンレティクル構造体
	struct LockOnReticle {
		std::unique_ptr<Reticle> reticle;
		Enemy* targetEnemy;
		Enemy* preTargetEnemy;
		bool isActive;
	};

	/// ロックされた敵構造体
	struct LockedEnemy {
		Enemy* enemy;
		bool isAssigned;
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// ロックオンシステムを初期化します。
	/// </summary>
	/// <remarks>
	/// - カメラとプレイヤーの参照を取得します。
	/// - メインレティクルを生成・初期化します。
	/// - プレイヤーからのオフセットと移動範囲・移動強度を設定します。
	/// - 最大ロックオン数分のロックオンレティクルを生成し、初期状態として非アクティブに設定します。
	/// </remarks>
	/// <param name="cameraPtr">使用するカメラへのポインタ</param>
	/// <param name="playerPtr">対象となるプレイヤーへのポインタ</param>
	void Initialize(Camera* cameraPtr, Player* playerPtr);

	/// <summary>
	/// ロックオンシステムを更新します。
	/// </summary>
	/// <remarks>
	/// - プレイヤーの移動に応じてメインレティクルの位置を補間して更新します。
	/// - メインレティクルが地面に埋まらないように高さを制限します。
	/// - 前回のターゲットを優先してロックオンレティクルに割り当てます。
	/// - 未割り当ての敵に対して残りのロックオンレティクルを割り当てます。
	/// - 各ロックオンレティクルのターゲット座標を設定し、アクティブ状態を更新します。
	/// - 更新後、ロックオン対象リストをクリアします。
	/// </remarks>
	void Update();

	/// <summary>
	/// メインおよびロックオンレティクルを描画します。
	/// </summary>
	/// <remarks>
	/// - メインレティクルを描画します。
	/// - 各ロックオンレティクルはアクティブ状態の場合のみ描画します。
	/// </remarks>
	void Draw();

	/// <summary>
	/// 指定した敵をロックオン対象に追加します。
	/// </summary>
	/// <remarks>
	/// - 既にロックオンされている敵は追加されません。
	/// - 新しい敵は未割り当て状態でリストに追加されます。
	/// </remarks>
	/// <param name="target">ロックオン対象とする敵のポインタ</param>
	void AddLockOnEnemy(Enemy* target);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//飛行機形態のレティクル
	std::unique_ptr<Reticle> mainReticle_;

	//戦車形態のレティクル
	std::list<std::unique_ptr<LockOnReticle>> lockOnReticles_;

	//ロックオンされた敵リスト
	std::list<LockedEnemy> lockedEnemies_;

	//カメラ
	Camera* camera_;

	//プレイヤー
	Player* player_;

	//オフセット
	Vector3 offset_;

	//移動範囲
	Vector3 moveRange_;

	//移動強度
	float moveStrength_;

	//最大ロックオン数
	int maxLockOnNum_;

	///-------------------------------------------/// 
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// メインレティクルの座標を取得します。
	/// </summary>
	/// <returns>メインレティクルの座標</returns>
	Vector3 GetMainReticlePos() const { return mainReticle_->GetWorldPos(); }

	/// <summary>
	/// アクティブなロックオンレティクルの座標を取得します。
	/// </summary>
	/// <remarks>
	/// - 現在アクティブ状態のロックオンレティクルのワールド座標を収集して返します。
	/// </remarks>
	/// <return>アクティブなロックオンレティクルの座標リスト</return>
	std::vector<Vector3> GetLockOnReticlePos();

};
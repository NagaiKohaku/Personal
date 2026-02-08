#pragma once

#include "Reticle.h"

#include "memory"
#include "vector"
#include "list"

class Player;
class Enemy;
class MyEngine::Camera;

/// <summary>
/// ロックオンシステムを管理するクラスです。
/// </summary>
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
	/// <param name="cameraPtr">使用するカメラへのポインタ</param>
	/// <param name="playerPtr">対象となるプレイヤーへのポインタ</param>
	void Initialize(MyEngine::Camera* cameraPtr, Player* playerPtr);

	/// <summary>
	/// ロックオンシステムを更新します。
	/// </summary>
	void Update();

	/// <summary>
	/// メインおよびロックオンレティクルを描画します。
	/// </summary>
	void Draw();

	/// <summary>
	/// 指定した敵をロックオン対象に追加します。
	/// </summary>
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
	MyEngine::Camera* camera_;

	//プレイヤー
	Player* player_;

	//オフセット
	MyEngine::Vector3 offset_;

	//移動範囲
	MyEngine::Vector3 moveRange_;

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
	MyEngine::Vector3 GetMainReticlePos() const { return mainReticle_->GetWorldPos(); }

	/// <summary>
	/// アクティブなロックオンレティクルの座標を取得します。
	/// </summary>
	/// <return>アクティブなロックオンレティクルの座標リスト</return>
	std::vector<MyEngine::Vector3> GetLockOnReticlePos();

	/// <summary>
	/// メインレティクルとエネミーの距離を取得します。
	/// </summary>
	/// <param name="enemy"></param>
	/// <returns></returns>
	float GetMainReticleToEnemyLength(Enemy* enemy);

};
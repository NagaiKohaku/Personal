#pragma once

#include <Object/Enemy/Enemy.h>

#include "3d/Camera/Camera.h"

#include "list"
#include "memory"

/// === 前方宣言 === ///

class LevelDataLoader;

class BulletManager;

class Player;

/// <summary>
/// 敵の生成・管理・削除を一括して行うクラスです。
/// </summary>
class EnemyManager {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 敵の生成管理に必要な各種パラメータや参照ポインタを初期化します。
	/// </summary>
	/// <param name="cameraPtr">追従や位置参照に使用するカメラのポインタ。</param>
	/// <param name="bulletPtr">敵が発射する弾を管理するバレットマネージャのポインタ。</param>
	/// <param name="playerPtr">敵が追跡・攻撃対象とするプレイヤーのポインタ。</param>
	void Initialize(Camera* cameraPtr, BulletManager* bulletPtr, Player* playerPtr);

	/// <summary>
	/// 敵の生成処理および各敵オブジェクトの更新処理を行います。
	/// </summary>
	void Update();

	/// <summary>
	/// 管理中の全ての敵オブジェクトに対して、座標変換のみの更新処理を行います。
	/// </summary>
	void TransformUpdate();

	/// <summary>
	/// 管理中の全ての敵オブジェクトを描画します。
	/// </summary>
	void Draw();

	/// <summary>
	/// 全敵に対してクリア演出を適用します。
	/// </summary>
	void StartClearUpdate();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 敵のスポーン処理を更新し、一定間隔で編成データに基づく複数の敵を出現させます。
	/// </summary>
	void SpawnUpdate();

	/// <summary>
	/// 敵を生成し、初期座標・エントリー座標・待機座標などの各種パラメータを設定します。
	/// </summary>
	/// <param name="entryPos">敵がステージ外から侵入してくるエントリー座標</param>
	/// <param name="standbyPos">侵入後に移動し停止する待機座標</param>
	/// <param name="objectData">敵の見た目やパラメータを含むオブジェクトデータ</param>
	void Spawn(Vector3 entryPos, Vector3 standbyPos, ObjectData objectData);

	/// <summary>
	/// 管理している敵オブジェクトの削除処理を行います。
	/// </summary>
	void DeleteEnemy();

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

	//レベルデータローダー
	LevelDataLoader* levelDataLoader_;

	//ディレクトリパス
	std::string directoryPath_;

	//エネミーリスト
	std::list<Enemy*> enemies_;

	//スポーンタイマー
	float spawnTimer_;

	//スポーン間隔
	float spawnInterval_;

	//スポーン最大数
	uint32_t spawnMaxSize_;

	//スポーンの基準方向
	float spawnBaseAngle_;

	//スポーン時の間隔距離
	float spawnDistance_;

	//スポーン座標オフセット
	Vector3 spawnOffset_;

	//スポーン範囲
	Vector3 spawnRange_;

	//スポーンフラグ
	bool isSpawn_;

public:

	/// <summary>
	/// エネミーリストのゲッター
	/// </summary>
	/// <returns>エネミーリスト</returns>
	std::list<Enemy*> GetEnemyList();

};
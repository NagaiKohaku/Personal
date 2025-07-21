#pragma once

#include "Enemy/Enemy.h"

#include "3d/Camera/Camera.h"

#include "list"
#include "memory"

/// === 前方宣言 === ///

class LevelDataLoader;

///=====================================================/// 
/// エネミーマネージャークラス
///=====================================================///
class EnemyManager {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="ptr">カメラポインタ</param>
	void Initialize(Camera* ptr);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// スポーン更新
	/// </summary>
	void SpawnUpdate();

	/// <summary>
	/// スポーン処理
	/// </summary>
	/// <param name="entryPos">出現位置</param>
	/// <param name="standbyPos">待機位置</param>
	void Spawn(Vector3 entryPos, Vector3 standbyPos, ObjectData objectData);

	/// <summary>
	/// エネミーの削除
	/// </summary>
	void DeleteEnemy();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	Camera* camera_;

	//レベルデータローダー
	LevelDataLoader* levelDataLoader_;

	//エネミーリスト
	std::list<std::unique_ptr<Enemy>> enemies_;

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
};
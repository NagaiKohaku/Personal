#pragma once

#include "Enemy/Enemy.h"

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
/// <remarks>
/// - ステージ上に出現する敵オブジェクトの生成・初期化を担当します。  
/// - 各敵の更新処理や描画処理をまとめて管理し、ゲームループ内で呼び出されます。  
/// - 編成データやランダム座標に基づき複数の敵をタイミングよく出現させます。  
/// - クリア演出や削除処理も統括して実行し、不要になった敵を安全にゲーム内から取り除きます。  
/// - カメラや弾管理、プレイヤー参照など、敵の挙動に必要な外部リソースを保持します。  
/// </remarks>
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
	/// <remarks>
	/// - レベルデータローダーを取得し、敵出現データの読み込み準備を行います。  
	/// - カメラ、弾管理、プレイヤーの各ポインタを保持します。  
	/// - 敵の出現に使用するディレクトリパスを設定します。  
	/// - 敵出現用タイマー、出現間隔、同時最大数などのスポーン管理パラメータを初期化します。  
	/// - 出現位置計算に使用する基準角度・距離・オフセットを設定します。  
	/// - スポーンを許可するフラグを初期化し、敵生成可能な状態で開始します。  
	/// </remarks>
	void Initialize(Camera* cameraPtr, BulletManager* bulletPtr, Player* playerPtr);

	/// <summary>
	/// 敵の生成処理および各敵オブジェクトの更新処理を行います。
	/// </summary>
	/// <remarks>
	/// - スポーンが許可されている場合、敵出現タイマーの更新と出現処理を実行します。  
	/// - 出現済みの敵の削除判定を行い、削除可能なものを適切に破棄します。  
	/// - 管理中の全ての敵に対して更新処理を呼び出し、挙動・座標・アニメーションなどを更新します。  
	/// </remarks>
	void Update();

	/// <summary>
	/// 管理中の全ての敵オブジェクトに対して、座標変換のみの更新処理を行います。
	/// </summary>
	/// <remarks>
	/// - 描画用の変換行列や座標情報のみを更新します。  
	/// - 挙動やアニメーションなどのロジック更新は行わず、位置同期のみを目的とした処理です。  
	/// </remarks>
	void TransformUpdate();

	/// <summary>
	/// 管理中の全ての敵オブジェクトを描画します。
	/// </summary>
	/// <remarks>
	/// - 各敵オブジェクトに対して個別の描画処理を呼び出します。  
	/// - 敵ごとに持つモデル、影、エミッターなどの描画を内部で実行します。  
	/// </remarks>
	void Draw();

	/// <summary>
	/// 全敵に対してクリア演出を適用します。
	/// </summary>
	/// <remarks>
	/// - 敵のスポーン処理を停止します。  
	/// - 全ての敵に対してクリア用爆発エミッターを起動します。  
	/// - クリア演出を強調するため、各敵に対して複数回のクリア更新処理を実行します。  
	/// </remarks>
	void StartClearUpdate();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 敵のスポーン処理を更新し、一定間隔で編成データに基づく複数の敵を出現させます。
	/// </summary>
	/// <remarks>
	/// - スポーン用タイマーを毎フレーム進行させます。  
	/// - タイマーがスポーン間隔を超えると、ランダムに編成データを選択します。  
	/// - ランダムな基準オフセット座標を生成し、編成データの敵配置位置に加算します。  
	/// - 各敵の待機座標からスポーン方向を算出し、一定距離手前の出現座標を決定します。  
	/// - 計算した出現座標・待機座標・編成データを基に個別の敵生成処理を呼び出します。  
	/// </remarks>
	void SpawnUpdate();

	/// <summary>
	/// 敵を生成し、初期座標・エントリー座標・待機座標などの各種パラメータを設定します。
	/// </summary>
	/// <param name="entryPos">敵がステージ外から侵入してくるエントリー座標</param>
	/// <param name="standbyPos">侵入後に移動し停止する待機座標</param>
	/// <param name="objectData">敵の見た目やパラメータを含むオブジェクトデータ</param>
	/// <remarks>
	/// - ObjectManager から敵インスタンスを生成します。  
	/// - 敵リストへ追加し、必要なマネージャやプレイヤー参照を渡して初期化します。  
	/// - エントリー→待機→離脱へ向かうための座標情報を設定します。  
	/// </remarks>
	void Spawn(Vector3 entryPos, Vector3 standbyPos, ObjectData objectData);

	/// <summary>
	/// 管理している敵オブジェクトの削除処理を行います。
	/// </summary>
	/// <remarks>
	/// - 各敵オブジェクトの削除可能状態を確認します。  
	/// - 削除可能な場合は、削除中の状態に変更した上で管理リストから除外します。  
	/// - これにより、不要になった敵オブジェクトを安全にゲーム内から取り除くことができます。  
	/// </remarks>
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
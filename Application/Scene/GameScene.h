#pragma once
#include "Scene/BaseScene.h"

#include "Base/Audio.h"
#include "2d/Object/Object2D.h"
#include "3d/Camera/Camera.h"
#include "3d/Object/Object3D.h"
#include "3d/Object/DebugLine.h"
#include "3d/Object/SkyBox.h"
#include "3d/Particle/ParticleEmitter.h"
#include "3d/Particle/EmitterGroup.h"

#include "Player/Player.h"
#include "Enemy/EnemyManager.h"
#include "Bullet/BulletManager.h"
#include "Camera/FollowCamera.h"
#include "Ground/LineGround.h"
#include "Ground/Ground.h"
#include "Ground/Building.h"
#include "Ground/GroundManager.h"

#include "memory"
#include "vector"
#include "list"

/// <summary>
/// ゲームシーンを表すクラスです。
/// </summary>
/// <remarks>
/// - BaseSceneを継承し、ゲームプレイに必要なオブジェクトの管理、更新、描画、アニメーション演出を行います。  
/// - カメラ（通常カメラ・追従カメラ）、プレイヤー、エネミー、弾、グラウンド、UIスプライト、エフェクトなどのオブジェクトを管理します。  
/// - ゲーム開始時・クリア時のアニメーションや、ゲームオーバー演出の制御も行います。  
/// - Update()やDraw()内で各オブジェクトの更新・描画を順序よく行い、シーン遷移やフェード制御も含めています。  
/// - AnimPoint構造体を用いたキーフレーム方式で、プレイヤーとカメラのアニメーションを補間します。  
/// </remarks>
class GameScene : public BaseScene {

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
public:

	//アニメーションのキーフレーム
	struct AnimPoint {
		Vector3 playerPos;
		Vector3 cameraRot;
		float time;
		float mag;
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// ゲームシーンの初期化を行います。
	/// </summary>
	/// <remarks>
	/// - カメラを生成・初期化し、デバッグカメラを無効化した後、シーン全体のカメラ制御に設定します。  
	/// - エネミーマネージャー、バレットマネージャー、グラウンドマネージャーを生成・初期化します。  
	/// - プレイヤーを生成し、カメラとバレットマネージャーを渡して初期化します。初期状態では移動不可に設定します。  
	/// - 追尾カメラを生成・初期化し、プレイヤーに追従させます。初期状態では無効化します。  
	/// - ゲームオーバーおよびゲームクリア関連の2Dスプライトを生成し、位置・アンカーポイント・カラーを設定します。  
	/// - 衝撃波エミッターを左右に生成・初期化し、カメラに紐づけて読み込みます。  
	/// - 各種タイマーやフラグを初期化し、シーン開始時のアニメーション用キーフレームを設定します。  
	/// - Fade制御にカメラとプレイヤーをセットし、フェードインを開始します。  
	/// </remarks>
	void Initialize() override;

	/// <summary>
	/// ゲームシーンの終了処理を行います。
	/// </summary>
	/// <remarks>
	/// - ObjectManagerに登録されている全てのオブジェクトをクリアします。  
	/// - 演出系（フェード・カメラシェイク）の参照をリセットし、外部オブジェクトへの依存を解除します。  
	/// - 使用している音声データやオーディオシステムを解放してリソースを整理します。  
	/// </remarks>
	void Finalize() override;

	/// <summary>
	/// ゲームシーンの毎フレーム更新処理を行います。
	/// </summary>
	/// <remarks>
	/// - フェード演出の状態を確認し、FADE_INが終了していれば状態をリセットします。  
	/// - スタートおよびクリア時のアニメーションを更新します。  
	/// - 追尾カメラおよびメインカメラを更新します。  
	/// - ゲーム進行中であれば、プレイヤー・エネミー・弾・グラウンドの各オブジェクトを更新します。  
	/// - ゲームクリア時にはTransformのみ更新して描画状態を維持します。  
	/// - UIの矢印アニメーションをタイマーで制御し、Lerp/EaseOutでスムーズに移動させます。  
	/// - ゲームオーバー・ゲームクリアスプライトの更新と表示制御を行います。  
	/// - 衝撃波エミッターを更新します。  
	/// - プレイヤーが死亡した場合、ゲームオーバー演出を開始します。  
	/// - 一定条件を満たした場合、ゲームクリア演出を開始し、フェードアウトによるシーン遷移を行います。  
	/// </remarks>
	void Update() override;

	/// <summary>
	/// ゲームシーンの描画処理を行います。
	/// </summary>
	/// <remarks>
	/// - プレイヤー、エネミー、弾、グラウンドをそれぞれ描画します。  
	/// - ゲームオーバー・ゲームクリア時のスプライトをUIレイヤーとして描画します。  
	/// - ゲームクリア演出中でなければ、ヘルプスプライトを描画します。  
	/// - 衝撃波エミッターを左右それぞれ描画します。  
	/// - 描画順は基本的にゲームオブジェクト → エフェクト → UIスプライトの順で行います。  
	/// </remarks>
	void Draw() override;

	/// <summary>
	/// デバッグ
	/// </summary>
	void ImGui() override;

	/// <summary>
	/// ゲーム開始時のカメラ・プレイヤーアニメーションを更新します。
	/// </summary>
	/// <remarks>
	/// - ゲーム開始演出が完了していない場合のみ処理を行います。  
	/// - タイマーを進め、現在のキーフレームに応じてプレイヤー位置とカメラ回転を補間します。  
	/// - キーフレーム間の補間にはEaseOut関数を使用し、滑らかな動きを実現します。  
	/// - 最終キーフレームに到達したら、開始演出を終了し、追従カメラを有効化、プレイヤー操作を可能にします。  
	/// </remarks>
	void StartAnimation();

	/// <summary>
	/// ゲームクリア時のプレイヤー・エフェクトアニメーションを更新します。
	/// </summary>
	/// <remarks>
	/// - クリア演出中のみ処理を行います。  
	/// - タイマーを進め、現在のキーフレームに応じてプレイヤー位置を補間します。  
	/// - プレイヤーの左右に衝撃波エミッターを追従させ、キーフレーム到達時に発射します。  
	/// - 最終キーフレームに到達したらフェードアウトを開始し、演出を終了します。  
	/// </remarks>
	void ClearAnimation();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	std::unique_ptr<Camera> camera_;

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	//ライン
	std::vector<std::unique_ptr<DebugLine>> lines_;

	//プレイヤー
	Player* player_;

	//エネミーマネージャー
	std::unique_ptr<EnemyManager> enemyManager_;

	//バレットマネージャー
	std::unique_ptr<BulletManager> bulletManager_;

	//グラウンドマネージャー
	std::unique_ptr<GroundManager> groundManager_;

	//衝撃波エミッター(左)
	std::unique_ptr<EmitterGroup> shockWaveLeftEmitter_;

	//衝撃波エミッター(右)
	std::unique_ptr<EmitterGroup> shockWaveRightEmitter_;

	//ライン描画の地面
	std::unique_ptr<LineGround> lineGround_;

	//SkyBox
	std::unique_ptr<SkyBox> skyBox_;

	//ゲームオーバースプライト
	std::unique_ptr<Object2D> gameOverSprite_;

	//ゲームオーバースペーススプライト
	std::unique_ptr<Object2D> gameOverSpaceSprite_;

	//ゲームオーバー左矢印スプライト
	std::unique_ptr<Object2D> gameOverLeftArrowSprite_;

	//ゲームオーバー右矢印スプライト
	std::unique_ptr<Object2D> gameOverRightArrowSprite_;

	//ゲームクリアスプライト
	std::unique_ptr<Object2D> gameClearSprite_;

	//ゲームクリアスペーススプライト
	std::unique_ptr<Object2D> gameClearSpaceSprite_;

	//ゲームクリア左矢印スプライト
	std::unique_ptr<Object2D> gameClearLeftArrowSprite_;

	//ゲームクリア右矢印スプライト
	std::unique_ptr<Object2D> gameClearRightArrowSprite_;

	//ヘルプスプライト
	std::unique_ptr<Object2D> helpSprite_;

	//スペースキースプライトの位置
	Vector2 spaceKeyPos_;

	//スペースキースプライトの大きさ
	Vector2 spaceKeySize_;

	//矢印の長さ
	float arrowLength_;

	//矢印のタイマー
	float arrowTimer_;

	//アニメーションタイマーの進行方向
	float timerDirection_;

	//アニメーションのタイマー
	float timer_;

	//キーフレームの番号
	int animNum_;

	//スタートアニメーションキーフレーム
	std::vector<AnimPoint> startAnimPoints_;

	//クリアアニメーションキーフレーム
	std::vector<AnimPoint> clearAnimPoints_;

	//スタート時の演出をするかのフラグ
	bool isStart_;

	//ゲームオーバーになったかのフラグ
	bool isGameOver_;

	//クリアになったかのフラグ
	bool isClear_;

	//クリアアニメーション中かのフラグ
	bool isClearAnim_;
};
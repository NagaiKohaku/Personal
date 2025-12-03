#pragma once

#include <Scene/BaseScene.h>

#include "3d/Camera/Camera.h"
#include "3d/Object/Object3D.h"
#include "2d/Object/Object2D.h"
#include "3d/Particle/EmitterGroup.h"
#include "Player/Player.h"
#include "Ground/GroundManager.h"

#include "memory"
#include "vector"

/// <summary>
/// タイトルシーンの処理と演出を管理するクラスです
/// </summary>
/// <remarks>
/// - 初期化から描画まで一連の流れを制御する  
/// - 入力に応じて開始演出や遷移演出を行う  
/// - 各オブジェクトの更新と表示を統括する
/// </remarks>
class TitleScene : public BaseScene {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// タイトルシーンの各種オブジェクトを初期化する関数です
	/// </summary>
	/// <remarks>
	/// - カメラ生成や設定を行い描画準備を整える  
	/// - モデルと2D画像を読み込み表示位置を設定する  
	/// - プレイヤーや地面を生成し初期状態を構築する  
	/// - エミッターを読み込んで作動準備を行う  
	/// - フェードやアニメ関連の値を初期化する
	/// </remarks>
	void Initialize() override;

	/// <summary>
	/// タイトルシーン終了時に各種オブジェクトを解放する関数です
	/// </summary>
	/// <remarks>
	/// - オブジェクト群を消去してメモリを解放する  
	/// - フェード用の参照を解除して依存をなくす  
	/// - シェイク用カメラを無効化して影響を止める
	/// </remarks>
	void Finalize() override;

	/// <summary>
	/// タイトルシーンの毎フレーム更新処理を行う関数です
	/// </summary>
	/// <remarks>
	/// - 入力監視し開始状態へ移行する  
	/// - カメラ回転や矢印演出を補間で制御する  
	/// - 各オブジェクトを更新し描画準備する  
	/// - フェード完了時にゲームシーンへ遷移する
	/// </remarks>
	void Update() override;

	/// <summary>
	/// タイトルシーンに必要な描画処理を行う関数です
	/// </summary>
	/// <remarks>
	/// - プレイヤーと地形を描画する  
	/// - エミッターの視覚効果を表示する  
	/// - タイトル関連のUIスプライトを描画する
	/// </remarks>
	void Draw() override;

	/// <summary>
	/// デバッグ
	/// </summary>
	void ImGui() override;

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// タイトル開始時の演出アニメーションを管理する関数です
	/// </summary>
	/// <remarks>
	/// - タイマーで区切った区間ごとに演出を進行する  
	/// - 補間で座標や色を変化させ演出を構成する  
	/// - 終盤で衝撃波発生とフェード開始を行う
	/// </remarks>
	void Start();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//カメラ
	std::unique_ptr<Camera> camera_;

	//プレイヤー
	Player* player_;

	//グラウンドマネージャー
	std::unique_ptr<GroundManager> groundManager_;

	//タイトル
	std::unique_ptr<Object2D> titleSprite_;

	//スペースキー
	std::unique_ptr<Object2D> spaceKeySprite_;

	//左矢印
	std::unique_ptr<Object2D> leftArrowSprite_;

	//右矢印
	std::unique_ptr<Object2D> rightArrowSprite_;

	//衝撃波エミッター(左)
	std::unique_ptr<EmitterGroup> shockWaveLeftEmitter_;

	//衝撃波エミッター(右)
	std::unique_ptr<EmitterGroup> shockWaveRightEmitter_;

	//アニメーション座標リスト
	std::vector<Vector3> animPos_;

	//アニメーション間隔リスト
	std::vector<float> animInterval;

	//カメラの回転
	Vector3 cameraRotate_;

	//スペースキーの座標
	Vector2 spaceKeyPos_;

	//スペースキーのサイズ
	Vector2 spaceKeySize_;

	//アニメーションタイマー
	float animTimer_;

	//アニメーション番号
	int animNum_;

	//矢印の長さ
	float arrowLength_;

	//矢印のタイマー
	float arrowTimer_;

	//タイマーの進行方向
	float timerDirection_;

	//スタートフラグ
	bool isStart_;

	//フェードフラグ
	bool isFade_;
};
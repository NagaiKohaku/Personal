#pragma once

#include <Scene/BaseScene.h>

#include "3d/Camera/Camera.h"
#include "3d/Object/Object3D.h"
#include "2d/Object/Object2D.h"
#include "3d/Particle/EmitterGroup.h"
#include <Object/Player/Player.h>
#include <Object/Ground/GroundManager.h>

#include "memory"
#include "vector"

/// <summary>
/// タイトルシーンの処理と演出を管理するクラスです
/// </summary>
class TitleScene : public MyEngine::BaseScene {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// タイトルシーンの各種オブジェクトを初期化する関数です
	/// </summary>
	void Initialize(MyEngine::EngineContext context) override;

	/// <summary>
	/// タイトルシーン終了時に各種オブジェクトを解放する関数です
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// タイトルシーンの毎フレーム更新処理を行う関数です
	/// </summary>
	void Update() override;

	/// <summary>
	/// タイトルシーンに必要な描画処理を行う関数です
	/// </summary>
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
	void Start();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//プレイヤー
	Player* player_;

	//グラウンドマネージャー
	std::unique_ptr<GroundManager> groundManager_;

	//衝撃波エミッター(左)
	std::unique_ptr<MyEngine::EmitterGroup> shockWaveLeftEmitter_;

	//衝撃波エミッター(右)
	std::unique_ptr<MyEngine::EmitterGroup> shockWaveRightEmitter_;

	//アニメーション座標リスト
	std::vector<MyEngine::Vector3> animPos_;

	//アニメーション間隔リスト
	std::vector<float> animInterval;

	//カメラの回転
	MyEngine::Vector3 cameraRotate_;

	//スペースキーの座標
	MyEngine::Vector2 spaceKeyPos_;

	//スペースキーのサイズ
	MyEngine::Vector2 spaceKeySize_;

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
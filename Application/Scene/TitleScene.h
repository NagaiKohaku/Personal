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

class TitleScene : public BaseScene {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// デバッグ
	/// </summary>
	void ImGui() override;

private:

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

	std::vector<Vector3> animPos_;

	std::vector<float> animInterval;

	//カメラの回転
	Vector3 cameraRotate_;

	Vector2 spaceKeyPos_;

	Vector2 spaceKeySize_;

	float animTimer_;

	int animNum_;

	float arrowLength_;

	float arrowTimer_;

	float timerDirection_;

	bool isStart_;

	bool isFade_;
};
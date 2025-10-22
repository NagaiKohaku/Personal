#pragma once

#include <2d/Object/Object2D.h>
#include <Math/WorldTransform.h>

#include <3d/Camera/Camera.h>
#include <Player/Player.h>

#include <memory>
#include <vector>

class Fade {

	public:

	enum FadeState {
		NONE,
		FADE_IN,
		FADE_IN_END,
		FADE_OUT,
		FADE_OUT_END
	};

	struct FadeSprite {
		std::unique_ptr<Object2D> sprite;
		Vector2 startSize;
		Vector2 endSize;
		float startTime;
		float endTime;
	};

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static Fade* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// フェードイン開始
	/// </summary>
	void StartFadeIn();

	/// <summary>
	/// フェードイン更新
	/// </summary>
	void FadeInUpdate();

	/// <summary>
	/// フェードアウト開始
	/// </summary>
	void StartFadeOut();

	/// <summary>
	/// フェードアウト更新
	/// </summary>
	void FadeOutUpdate();

	///=====================================================/// 
	/// ゲッター・セッター
	///=====================================================///
public:

	/// <summary>
	/// フェード状態の取得
	/// </summary>
	FadeState GetState() const { return state_; }

	/// <summary>
	/// カメラの取得
	/// </summary>
	Camera* GetCamera() { return camera_; }

	/// <summary>
	/// プレイヤーの取得
	/// </summary>
	Player* GetPlayer() { return player_; }

	/// <summary>
	/// フェード状態の設定
	/// </summary>
	void SetState(FadeState state) { state_ = state; }

	/// <summary>
	/// カメラの設定
	/// </summary>
	void SetCamera(Camera* ptr) { camera_ = ptr; }

	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	void SetPlayer(Player* ptr) { player_ = ptr; }

	///=====================================================/// 
	/// クラス内関数
	///=====================================================///
private:

	/// <summary>
	/// リングスプライトの生成
	/// </summary>
	void CreateRingSprite();

	/// <summary>
	/// 円形スプライトの生成
	/// </summary>
	void CreateCircleSprite();

	///=====================================================/// 
	/// メンバ変数
	///=====================================================///
private:

	//フェード状態
	FadeState state_ = FadeState::NONE;

	//フェードスプライト
	std::vector<FadeSprite> fadeSprites_;

	//カメラ
	Camera* camera_;

	//プレイヤー
	Player* player_;

	//プレイヤーの2D座標
	Vector2 playerPos2D_;

	//アルファ値
	float alpha_ = 0.0f;

	//タイマー
	float timer_ = 0.0f;

	//最大タイマー
	float maxTimer_ = 1.0f;

	//フェード速度
	float fadeSpeed_ = 0.02f;
};
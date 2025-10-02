#pragma once

#include <2d/Object/Object2D.h>

#include <memory>

class Fade {

	public:

	enum FadeState {
		NONE,
		FADE_IN,
		FADE_OUT
	};

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

	/// <summary>
	/// フェード状態の取得
	/// </summary>
	FadeState GetState() const { return state_; }

private:

	//フェード状態
	FadeState state_ = FadeState::NONE;

	//フェードスプライト
	std::unique_ptr<Object2D> fadeSprite_;

	//アルファ値
	float alpha_ = 0.0f;

	//タイマー
	float timer_ = 0.0f;

	//最大タイマー
	float maxTimer_ = 1.0f;

	//フェード速度
	float fadeSpeed_ = 0.02f;
};
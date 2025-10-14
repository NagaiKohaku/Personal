#include "Fade.h"

#include <2d/Sprite/SpriteManager.h>

Fade* Fade::GetInstance() {
	static Fade instance;
	return &instance;
}

/// <summary>
/// 初期化
/// </summary>
void Fade::Initialize() {

	SpriteManager::GetInstance()->LoadSprite("whiteCube", "white_128x128");

	fadeSprite_ = std::make_unique<Object2D>();

	fadeSprite_->Initialize();

	fadeSprite_->SetSprite("whiteCube");

	fadeSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	fadeSprite_->GetSprite()->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	fadeSprite_->SetSize({ 1280.0f,720.0f });

	fadeSprite_->SetTranslate({ 640.0f,360.0f });
}

/// <summary>
/// 更新
/// </summary>
void Fade::Update(){

	FadeInUpdate();

	FadeOutUpdate();

	fadeSprite_->Update();
}

/// <summary>
/// 描画
/// </summary>
void Fade::Draw(){

	fadeSprite_->Draw(LayerType::Last);
}

/// <summary>
/// フェードイン開始
/// </summary>
void Fade::StartFadeIn(){

	state_ = FadeState::FADE_IN;

	alpha_ = 1.0f;

	timer_ = 0.0f;
}

/// <summary>
/// フェードイン更新
/// </summary>
void Fade::FadeInUpdate(){

	if (state_ == FadeState::FADE_IN) {

		timer_ += fadeSpeed_;

		alpha_ = 1.0f - timer_ / maxTimer_;

		if (alpha_ <= 0.0f) {

			alpha_ = 0.0f;

			state_ = FadeState::NONE;
		}

		fadeSprite_->GetSprite()->SetColor({ 0.0f,0.0f,0.0f,alpha_ });
	}
}

/// <summary>
/// フェードアウト開始
/// </summary>
void Fade::StartFadeOut(){

	state_ = FadeState::FADE_OUT;

	alpha_ = 0.0f;

	timer_ = 0.0f;
}

/// <summary>
/// フェードアウト更新
/// </summary>
void Fade::FadeOutUpdate(){

	if (state_ == FadeState::FADE_OUT) {

		timer_ += fadeSpeed_;

		alpha_ = timer_ / maxTimer_;

		if (alpha_ >= 1.0f) {

			alpha_ = 1.0f;

			state_ = FadeState::NONE;
		}

		fadeSprite_->GetSprite()->SetColor({ 0.0f,0.0f,0.0f,alpha_ });
	}
}
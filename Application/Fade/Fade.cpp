#include "Fade.h"

#include <2d/Sprite/SpriteManager.h>

#include <Math/Random.h>
#include <Math/Easing.h>

Fade* Fade::GetInstance() {
	static Fade instance;
	return &instance;
}

/// <summary>
/// 初期化
/// </summary>
void Fade::Initialize() {

	SpriteManager::GetInstance()->LoadSprite("Ring", "BigRing");

	SpriteManager::GetInstance()->LoadSprite("Circle", "BigCircle");

	for (int i = 0; i < 3; i++) {

		CreateFadeSprite();
	}

	std::unique_ptr<Object2D> newObject;

	newObject = std::make_unique<Object2D>();

	newObject->Initialize();

	newObject->SetSprite("Circle");

	newObject->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	newObject->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	newObject->SetSize({ 0.0f,0.0f });

	newObject->SetTranslate({ 640.0f,360.0f });

	Vector2 startSize = { 0.0f,0.0f };

	Vector2 endSize = { 2000.0f,2000.0f };

	float startTime = static_cast<float>(fadeSprites_.back().endTime) - 0.8f;

	float endTime = startTime + 1.0f;

	fadeSprites_.push_back({ std::move(newObject),startSize,endSize,startTime,endTime });
}

/// <summary>
/// 更新
/// </summary>
void Fade::Update() {

	FadeInUpdate();

	FadeOutUpdate();

	for (auto& fadeSprite : fadeSprites_) {

		fadeSprite.sprite->Update();
	}

}

/// <summary>
/// 描画
/// </summary>
void Fade::Draw() {

	for (auto& fadeSprite : fadeSprites_) {

		fadeSprite.sprite->Draw(LayerType::UI);
	}
}

/// <summary>
/// フェードイン開始
/// </summary>
void Fade::StartFadeIn() {

	state_ = FadeState::FADE_IN;

	timer_ = 0.0f;

	for(auto& fadeSprite : fadeSprites_) {
		fadeSprite.sprite->SetSize(fadeSprite.endSize);

	}
}

/// <summary>
/// フェードイン更新
/// </summary>
void Fade::FadeInUpdate() {

	if (state_ == FadeState::FADE_IN) {

		timer_ += 1.0f / 60.0f;

		for (auto& fadeSprite : fadeSprites_) {

			if (timer_ >= fadeSprite.startTime) {

				float t = (timer_ - fadeSprite.startTime) / (fadeSprite.endTime - fadeSprite.startTime);

				if (t >= 1.0f) {

					t = 1.0f;
				}

				Vector2 size = EaseOut(fadeSprite.endSize, fadeSprite.startSize, t, 2.0f);

				fadeSprite.sprite->SetSize(size);
			}
		}

		if (timer_ >= fadeSprites_.back().endTime) {

			timer_ = 0.0f;

			state_ = FadeState::FADE_IN_END;
		}
	}
}

/// <summary>
/// フェードアウト開始
/// </summary>
void Fade::StartFadeOut() {

	state_ = FadeState::FADE_OUT;

	timer_ = 0.0f;

	for (auto& fadeSprite : fadeSprites_) {
		fadeSprite.sprite->SetSize(fadeSprite.startSize);
	}
}

/// <summary>
/// フェードアウト更新
/// </summary>
void Fade::FadeOutUpdate() {

	if (state_ == FadeState::FADE_OUT) {

		timer_ += 1.0f / 60.0f;

		for (auto& fadeSprite : fadeSprites_) {

			if (timer_ >= fadeSprite.startTime) {

				float t = (timer_ - fadeSprite.startTime) / (fadeSprite.endTime - fadeSprite.startTime);

				if (t >= 1.0f) {

					t = 1.0f;
				}

				Vector2 size = EaseOut(fadeSprite.startSize, fadeSprite.endSize, t, 2.0f);

				fadeSprite.sprite->SetSize(size);
			}
		}

		if (timer_ >= fadeSprites_.back().endTime) {

			timer_ = 0.0f;

			state_ = FadeState::FADE_OUT_END;
		}
	}
}

void Fade::CreateFadeSprite() {

	std::unique_ptr<Object2D> newObject;

	newObject = std::make_unique<Object2D>();

	newObject->Initialize();

	newObject->SetSprite("Ring");

	newObject->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	newObject->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	newObject->SetSize({ 256.0f,256.0f });

	newObject->SetTranslate({ 640.0f,360.0f });

	Vector2 startSize = { 0.0f,0.0f };

	Vector2 endSize = { 2560.0f,2560.0f };

	if (fadeSprites_.size() == 0) {

		float startTime = 0.0f;

		float endTime = startTime + 1.0f;

		fadeSprites_.push_back({ std::move(newObject),startSize,endSize,startTime,endTime });
	} else {

		float startTime = static_cast<float>(fadeSprites_.back().endTime) - 0.8f;

		float endTime = startTime + 1.0f;

		fadeSprites_.push_back({ std::move(newObject),startSize,endSize,startTime,endTime });
	}
}
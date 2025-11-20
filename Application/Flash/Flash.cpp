#include "Flash.h"

#include "2d/Sprite/SpriteManager.h"

/// <summary>
/// シングルトンインスタンスの取得
/// </summary>
Flash* Flash::GetInstance() {
	static Flash instance;
	return &instance;
}

/// <summary>
/// 初期化
/// </summary>
void Flash::Initialize() {

	SpriteManager::GetInstance()->LoadSprite("Flash", "white_128x128");

	//フラッシュ用オブジェクトの生成
	flashObject_ = std::make_unique<Object2D>();

	flashObject_->Initialize();

	flashObject_->SetSprite("Flash");

	flashObject_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	flashObject_->SetTranslate({ 640.0f,360.0f });

	flashObject_->SetSize({ 1280.0f,720.0f });

	flashObject_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,0.0f });

	alpha_ = 0.0f;

	isFlash_ = false;

	timer_ = 0.0f;

	maxTime_ = 0.0f;

	color_ = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
}

/// <summary>
/// 更新
/// </summary>
void Flash::Update() {

	if (isFlash_) {

		timer_ += 1.0f / 60.0f;

		alpha_ = 1.0f - timer_ / maxTime_;

		if (alpha_ <= 0.0f) {

			alpha_ = 0.0f;

			isFlash_ = false;
		}

		flashObject_->GetSprite()->SetColor({ color_.x,color_.y,color_.z,alpha_ });
	}

	flashObject_->Update();
}

/// <summary>
/// 描画
/// </summary>
void Flash::Draw() {

	flashObject_->Draw(LayerType::UI);
}

/// <summary>
/// 開始
/// </summary>
/// <param name="maxTime"></param>
void Flash::Start(float maxTime, Vector4 color) {

	alpha_ = 1.0f;

	timer_ = 0.0f;

	maxTime_ = maxTime;

	isFlash_ = true;

	color_ = color;
}
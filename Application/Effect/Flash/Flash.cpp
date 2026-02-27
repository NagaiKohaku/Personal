#include "Flash.h"

#include "2d/Sprite/SpriteManager.h"

using namespace MyEngine;

///=====================================================/// 
/// Flashのシングルトンインスタンスを取得
///=====================================================///
Flash* Flash::GetInstance() {
	static Flash instance;
	return &instance;
}

///=====================================================/// 
/// フラッシュエフェクトの初期化
///=====================================================///
void Flash::Initialize(Object2DCommon* object2DCommonPtr, Camera* cameraPtr, Renderer* rendererPtr) {

	camera_ = cameraPtr;

	renderer_ = rendererPtr;

	//フラッシュ用オブジェクトの生成
	flashObject_ = std::make_unique<Object2D>();

	flashObject_->Initialize(object2DCommonPtr, camera_, renderer_);

	flashObject_->SetSprite("white_128x128");

	flashObject_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	flashObject_->SetTranslate({ 640.0f,360.0f });

	flashObject_->SetSize({ 1280.0f,720.0f });

	flashObject_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,0.0f });

	//透明度初期化
	alpha_ = 0.0f;

	//フラッシュ中フラグ初期化
	isFlash_ = false;

	//タイマー初期化
	timer_ = 0.0f;

	//最大時間初期化
	maxTime_ = 0.0f;

	//フラッシュ色初期化
	color_ = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
}

///=====================================================/// 
/// フラッシュエフェクトの更新処理
///=====================================================///
void Flash::Update() {

	//フラッシュ中なら
	if (isFlash_) {

		//タイマー更新
		timer_ += 1.0f / 60.0f;

		//アルファ値更新
		alpha_ = 1.0f - timer_ / maxTime_;

		//アルファ値が0以下ならフラッシュ終了
		if (alpha_ <= 0.0f) {

			alpha_ = 0.0f;

			isFlash_ = false;
		}

		//スプライトの色更新
		flashObject_->GetSprite()->SetColor({ color_.x,color_.y,color_.z,alpha_ });
	}

	//フラッシュ用オブジェクト更新
	flashObject_->Update();
}

///=====================================================/// 
/// フラッシュエフェクトを描画
///=====================================================///
void Flash::Draw() {

	//フラッシュ用オブジェクト描画
	flashObject_->Draw(LayerType::UI);
}

///=====================================================/// 
/// フラッシュエフェクトを開始
///=====================================================///
/// <param name="maxTime"></param>
void Flash::Start(float maxTime, Vector4 color) {

	//透明度初期化
	alpha_ = 1.0f;

	//タイマー初期化
	timer_ = 0.0f;

	//最大時間設定
	maxTime_ = maxTime;

	//フラッシュ中フラグ設定
	isFlash_ = true;

	//フラッシュ色設定
	color_ = color;
}
#include "Fade.h"

#include <2d/Sprite/SpriteManager.h>
#include <Math/Utility/MakeMatrixMath.h>

#include <Math/Utility/Random.h>
#include <Math/Utility/Easing.h>

using namespace MyEngine;

///=====================================================/// 
/// Fadeのシングルトンインスタンスを取得
///=====================================================///
Fade* Fade::GetInstance() {
	static Fade instance;
	return &instance;
}

///=====================================================/// 
/// フェード処理に必要なスプライトやリソースを初期化
///=====================================================///
void Fade::Initialize(WinApp* winAppPtr, Object2DCommon* object2DCommonPtr, Camera* cameraPtr, Renderer* rendererPtr) {

	winApp_ = winAppPtr;
	camera_ = cameraPtr;
	renderer_ = rendererPtr;

	/// === フェードに使うスプライトの生成 === ///

	//リングスプライトの生成
	for (int i = 0; i < 3; i++) {

		CreateRingSprite(object2DCommonPtr);
	}

	//円形スプライトの生成
	CreateCircleSprite(object2DCommonPtr);
}

///=====================================================/// 
/// フェード処理の状態を毎フレーム更新
///=====================================================///
void Fade::Update() {

	//フェードインの更新
	FadeInUpdate();

	//フェードアウトの更新
	FadeOutUpdate();

	//カメラとプレイヤーが設定されていれば更新
	if (camera_ != nullptr) {
		if (player_ != nullptr) {

			//プレイヤーの位置を2D座標に変換
			Matrix4x4 viewport = camera_->GetViewPortMatrix();

			Matrix4x4 viewProjectionViewport = camera_->Get3DViewProjectionMatrix() * viewport;

			Vector3 screenPos = Transform(player_->GetWorldPos(), viewProjectionViewport);

			//座標が吹っ飛んでいなければ座標を設定
			if (screenPos.x != 0.0f) {
				if (screenPos.y != 0.0f) {

					playerPos2D_ = { screenPos.x,screenPos.y };
				}
			}
		}
	}

	for (auto& fadeSprite : fadeSprites_) {

		//スプライトの更新
		fadeSprite.sprite->Update();
	}

}

///=====================================================/// 
/// 管理しているフェード用スプライトを画面に描画
///=====================================================///
void Fade::Draw() {

	for (auto& fadeSprite : fadeSprites_) {

		//スプライトの描画
		fadeSprite.sprite->Draw(LayerType::UI);
	}
}

///=====================================================/// 
/// フェードイン処理を開始
///=====================================================///
void Fade::StartFadeIn() {

	//フェードイン状態にして更新開始
	state_ = FadeState::FADE_IN;

	//タイマーのリセット
	timer_ = 0.0f;

	//スプライトの初期座標・初期サイズの設定
	for (auto& fadeSprite : fadeSprites_) {

		fadeSprite.sprite->SetTranslate(Vector2(winApp_->GetWindowWidth() / 2.0f,winApp_->GetWindowHeight() / 2.0f));

		fadeSprite.sprite->SetSize(fadeSprite.endSize);

		//減算で計算するので、最後のスプライトの終了時間を設定
		timer_ = fadeSprite.endTime;
	}
}

///=====================================================/// 
/// フェードインの進行処理
///=====================================================///
void Fade::FadeInUpdate() {

	if (state_ == FadeState::FADE_IN) {

		//タイマーを減算
		timer_ -= 1.0f / 60.0f;

		for (auto& fadeSprite : fadeSprites_) {

			//開始時間・終了時間の間のみ更新
			if (timer_ >= fadeSprite.startTime && timer_ <= fadeSprite.endTime) {

				//経過時間の比率
				float t = (timer_ - fadeSprite.startTime) / (fadeSprite.endTime - fadeSprite.startTime);

				//下限値補正
				if (t <= 0.0f) {

					t = 0.0f;
				}

				//イージング計算
				Vector2 size = EaseIn(fadeSprite.startSize, fadeSprite.endSize, t, 2.0f);

				//サイズを設定
				fadeSprite.sprite->SetSize(size);

				//座標を設定
				fadeSprite.sprite->SetTranslate(playerPos2D_);

			}
		}

		//タイマーが終了時間になったらフェードイン終了
		if (timer_ <= fadeSprites_.front().startTime) {

			timer_ = 0.0f;

			state_ = FadeState::FADE_IN_END;
		}
	}
}

///=====================================================/// 
/// フェードアウト処理を開始
///=====================================================///
void Fade::StartFadeOut() {

	//フェードアウト状態にして更新開始
	state_ = FadeState::FADE_OUT;

	//タイマーのリセット
	timer_ = 0.0f;

	//スプライトの初期座標・初期サイズの設定
	for (auto& fadeSprite : fadeSprites_) {
		fadeSprite.sprite->SetTranslate(Vector2(winApp_->GetWindowWidth() / 2.0f, winApp_->GetWindowHeight() / 2.0f));
		fadeSprite.sprite->SetSize(fadeSprite.startSize);
	}
}

///=====================================================/// 
/// フェードアウトの進行処理
///=====================================================///
void Fade::FadeOutUpdate() {

	if (state_ == FadeState::FADE_OUT) {

		//タイマーを加算
		timer_ += 1.0f / 60.0f;

		for (auto& fadeSprite : fadeSprites_) {

			//開始時間・終了時間の間のみ更新
			if (timer_ >= fadeSprite.startTime && timer_ <= fadeSprite.endTime) {

				//経過時間の比率
				float t = (timer_ - fadeSprite.startTime) / (fadeSprite.endTime - fadeSprite.startTime);

				//上限値補正e
				if (t >= 1.0f) {

					t = 1.0f;
				}

				//イージング計算
				Vector2 size = EaseOut(fadeSprite.startSize, fadeSprite.endSize, t, 2.0f);

				//サイズを設定
				fadeSprite.sprite->SetSize(size);

				//座標を設定
				fadeSprite.sprite->SetTranslate(playerPos2D_);

			}
		}

		//タイマーが終了時間になったらフェードアウト終了
		if (timer_ >= fadeSprites_.back().endTime) {

			timer_ = 0.0f;

			state_ = FadeState::FADE_OUT_END;
		}
	}
}

///=====================================================/// 
/// リング形状のスプライトを生成し、フェード用の管理リストに追加
///=====================================================///
void Fade::CreateRingSprite(Object2DCommon* object2DCommonPtr) {

	//新しいオブジェクトを生成
	std::unique_ptr<Object2D> newObject;

	//オブジェクトを生成してパラメータの初期化をする
	newObject = std::make_unique<Object2D>();

	newObject->Initialize(object2DCommonPtr, camera_, renderer_);

	//リングスプライトをセット
	newObject->SetSprite("BigRing");

	newObject->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	newObject->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	newObject->SetSize({ 0.0f,0.0f });

	newObject->SetTranslate({ 640.0f,360.0f });

	Vector2 startSize = { 0.0f,0.0f };

	Vector2 endSize = { 3000.0f,3000.0f };

	//最初のスプライトだったら
	if (fadeSprites_.size() == 0) {

		//0からタイマーを開始
		float startTime = 0.0f;

		float endTime = startTime + 1.0f;

		fadeSprites_.push_back({ std::move(newObject),startSize,endSize,startTime,endTime });
	} else {

		//最初以外であれば前のスプライトの終了時間から開始
		float startTime = static_cast<float>(fadeSprites_.back().endTime) - 0.8f;

		float endTime = startTime + 1.0f;

		fadeSprites_.push_back({ std::move(newObject),startSize,endSize,startTime,endTime });
	}
}

///=====================================================/// 
/// 円形のスプライトを生成し、フェード用の管理リストに追加
///=====================================================///
void Fade::CreateCircleSprite(Object2DCommon* object2DCommonPtr) {

	//新しいオブジェクトを生成
	std::unique_ptr<Object2D> newObject;

	//オブジェクトを生成してパラメータの初期化をする
	newObject = std::make_unique<Object2D>();

	newObject->Initialize(object2DCommonPtr, camera_, renderer_);

	//円形スプライトをセット
	newObject->SetSprite("BigCircle");

	newObject->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	newObject->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	newObject->SetSize({ 0.0f,0.0f });

	newObject->SetTranslate({ 640.0f,360.0f });

	Vector2 startSize = { 0.0f,0.0f };

	Vector2 endSize = { 3000.0f,3000.0f };

	float startTime = static_cast<float>(fadeSprites_.back().endTime) - 0.8f;

	float endTime = startTime + 1.0f;

	fadeSprites_.push_back({ std::move(newObject),startSize,endSize,startTime,endTime });
}
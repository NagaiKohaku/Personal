#include "GameScene2DObject.h"

#include <2d/Sprite/SpriteManager.h>
#include <Math/Easing.h>

void GameScene2DObject::Initialize() {

	/// === スプライトの読み込み === ///

	SpriteManager::GetInstance()->LoadSprite("GameOver", "RoadFlightGameOver");

	SpriteManager::GetInstance()->LoadSprite("GameOverSpace", "GameOverSpace");

	SpriteManager::GetInstance()->LoadSprite("GameOverArrow", "GameOverArrow");

	SpriteManager::GetInstance()->LoadSprite("GameClear", "RoadFlightGameClear");

	SpriteManager::GetInstance()->LoadSprite("GameClearSpace", "GameClearSpace");

	SpriteManager::GetInstance()->LoadSprite("GameClearArrow", "GameClearArrow");

	SpriteManager::GetInstance()->LoadSprite("KillToTitle", "KillToTitle");

	/// === ゲームオーバースプライトの生成 === ///

	//テキストスプライトの生成
	gameOverSprite_ = std::make_unique<Object2D>();

	gameOverSprite_->Initialize();

	gameOverSprite_->SetSprite("GameOver");

	gameOverSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameOverSprite_->SetTranslate({ 640.0f,100.0f });

	gameOverSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	//スペースキースプライトの生成
	gameOverSpaceSprite_ = std::make_unique<Object2D>();

	gameOverSpaceSprite_->Initialize();

	gameOverSpaceSprite_->SetSprite("GameOverSpace");

	gameOverSpaceSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameOverSpaceSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	gameOverSpaceSprite_->SetTranslate({ 640.0f,600.0f });

	gameOverSpaceSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	spaceKeyPos_ = gameOverSpaceSprite_->GetTranslate();

	spaceKeySize_ = gameOverSpaceSprite_->GetSize();

	//左矢印スプライトの生成
	gameOverLeftArrowSprite_ = std::make_unique<Object2D>();

	gameOverLeftArrowSprite_->Initialize();

	gameOverLeftArrowSprite_->SetSprite("GameOverArrow");

	gameOverLeftArrowSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameOverLeftArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	gameOverLeftArrowSprite_->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f,spaceKeyPos_.y });

	gameOverLeftArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	//右矢印スプライトの生成
	gameOverRightArrowSprite_ = std::make_unique<Object2D>();

	gameOverRightArrowSprite_->Initialize();

	gameOverRightArrowSprite_->SetSprite("GameOverArrow");

	gameOverRightArrowSprite_->GetSprite()->SetIsFlipX(true);

	gameOverRightArrowSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameOverRightArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	gameOverRightArrowSprite_->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f,spaceKeyPos_.y });

	gameOverRightArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	/// === ゲームクリアスプライトの生成 === ///

	//テキストスプライトの生成
	gameClearSprite_ = std::make_unique<Object2D>();

	gameClearSprite_->Initialize();

	gameClearSprite_->SetSprite("GameClear");

	gameClearSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameClearSprite_->SetTranslate({ 640.0f,100.0f });

	gameClearSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	//スペースキースプライトの生成
	gameClearSpaceSprite_ = std::make_unique<Object2D>();

	gameClearSpaceSprite_->Initialize();

	gameClearSpaceSprite_->SetSprite("GameClearSpace");

	gameClearSpaceSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameClearSpaceSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	gameClearSpaceSprite_->SetTranslate({ 640.0f,600.0f });

	gameClearSpaceSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	//左矢印スプライトの生成
	gameClearLeftArrowSprite_ = std::make_unique<Object2D>();

	gameClearLeftArrowSprite_->Initialize();

	gameClearLeftArrowSprite_->SetSprite("GameClearArrow");

	gameClearLeftArrowSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameClearLeftArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	gameClearLeftArrowSprite_->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f,spaceKeyPos_.y });

	gameClearLeftArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	//右矢印スプライトの生成
	gameClearRightArrowSprite_ = std::make_unique<Object2D>();

	gameClearRightArrowSprite_->Initialize();

	gameClearRightArrowSprite_->SetSprite("GameClearArrow");

	gameClearRightArrowSprite_->GetSprite()->SetIsFlipX(true);

	gameClearRightArrowSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	gameClearRightArrowSprite_->GetSprite()->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	gameClearRightArrowSprite_->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f,spaceKeyPos_.y });

	gameClearRightArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	/// === ヘルプスプライトの生成 === ///

	helpSprite_ = std::make_unique<Object2D>();

	helpSprite_->Initialize();

	helpSprite_->SetSprite("KillToTitle");

	helpSprite_->GetSprite()->SetAnchorPoint({ 0.5f,0.5f });

	helpSprite_->SetTranslate({ 640.0f,360.0f });

	/// === 他変数の初期化 === ///

	arrowLength_ = 20.0f;

	arrowTimer_ = 0.0f;

	timerDirection_ = 1.0f;

}

void GameScene2DObject::Update() {

	arrowTimer_ += (1.0f / 60.0f) * timerDirection_;

	if (arrowTimer_ >= 1.0f) {

		arrowTimer_ = 1.0f;

		timerDirection_ *= -1.0f;
	}

	if (arrowTimer_ <= 0.0f) {

		arrowTimer_ = 0.0f;

		timerDirection_ *= -1.0f;
	}

	float lerpNum = EaseOut(0.0f, arrowLength_, arrowTimer_ / 1.0f, 2.0f);

	gameOverLeftArrowSprite_->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f - lerpNum,spaceKeyPos_.y });

	gameOverRightArrowSprite_->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f + lerpNum,spaceKeyPos_.y });

	gameClearLeftArrowSprite_->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f - lerpNum,spaceKeyPos_.y });

	gameClearRightArrowSprite_->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f + lerpNum,spaceKeyPos_.y });

	//ゲームオーバースプライトの更新
	gameOverSprite_->Update();
	gameOverSpaceSprite_->Update();
	gameOverLeftArrowSprite_->Update();
	gameOverRightArrowSprite_->Update();

	//ゲームクリアスプライトの更新
	gameClearSprite_->Update();
	gameClearSpaceSprite_->Update();
	gameClearLeftArrowSprite_->Update();
	gameClearRightArrowSprite_->Update();

	helpSprite_->Update();
}

void GameScene2DObject::Draw() {

	//ゲームオーバースプライトの更新
	gameOverSprite_->Draw(LayerType::UI);
	gameOverSpaceSprite_->Draw(LayerType::UI);
	gameOverLeftArrowSprite_->Draw(LayerType::UI);
	gameOverRightArrowSprite_->Draw(LayerType::UI);

	//ゲームクリアスプライトの更新
	gameClearSprite_->Draw(LayerType::UI);
	gameClearSpaceSprite_->Draw(LayerType::UI);
	gameClearLeftArrowSprite_->Draw(LayerType::UI);
	gameClearRightArrowSprite_->Draw(LayerType::UI);

	if (!isClear_ && !isClearAnim_) {

		helpSprite_->Draw(LayerType::UI);
	}

}

void GameScene2DObject::HideSprite() {

	//スプライトを隠す
	gameClearSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	gameClearSpaceSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	gameClearLeftArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

	gameClearRightArrowSprite_->GetSprite()->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));

}

void GameScene2DObject::ShowSprite() {

	//スプライトを映す
	gameClearSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	gameClearSpaceSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	gameClearLeftArrowSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	gameClearRightArrowSprite_->GetSprite()->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}
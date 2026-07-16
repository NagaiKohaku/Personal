#include "GameOverEvent.h"

#include <UI/UIManager.h>
#include <Effect/Shake/Shake.h>
#include <Effect/Fade/Fade.h>
#include <Base/OffScreen.h>
#include <Base/Input.h>

using namespace MyEngine;

void GameOverEvent::Start(MyEngine::EngineContext context, Player* player, FollowCamera* followCamera) {

	context_ = context;

	//スプライトを映す
	UIManager::GetInstance()->GetUIGroup("GameOver")->isActive = true;

	//テキスト系のUIを隠す
	UIManager::GetInstance()->GetUIGroup("Reticle")->isActive = false;

	UIManager::GetInstance()->GetUIGroup("Help")->isActive = false;

	spaceKeyPos_ = UIManager::GetInstance()->Get2DObject("GameOver", "Space")->GetTranslate();

	spaceKeySize_ = UIManager::GetInstance()->Get2DObject("GameOver", "Space")->GetSize();

	//シェイクを始める
	Shake::GetInstance()->Start(1.0f, 0.5f);

	//色を反転させる
	context_.offScreen->SetColorReverseRatio(1.0f);

	arrowLength_ = 20.0f;

	arrowTimer_ = 0.0f;

	timerDirection_ = 1.0f;
}

void GameOverEvent::Exit() {
}

void GameOverEvent::Update() {

	arrowTimer_ += (1.0f / 60.0f) * timerDirection_;

	if (arrowTimer_ >= 1.0f) {

		arrowTimer_ = 1.0f;

		timerDirection_ *= -1.0f;
	}

	if (arrowTimer_ <= 0.0f) {

		arrowTimer_ = 0.0f;

		timerDirection_ *= -1.0f;
	}

	float lerpNum = EaseOut(0.0f, arrowLength_, arrowTimer_ / 1.0f);

	UIManager::GetInstance()->Get2DObject("GameOver", "LeftArrow")->SetTranslate({ spaceKeyPos_.x - spaceKeySize_.x / 2.0f - 64.0f - lerpNum,spaceKeyPos_.y });

	UIManager::GetInstance()->Get2DObject("GameOver", "RightArrow")->SetTranslate({ spaceKeyPos_.x + spaceKeySize_.x / 2.0f + 64.0f + lerpNum,spaceKeyPos_.y });

	if (context_.input->IsTriggerPushKey(DIK_SPACE)) {

		if (!isFade_) {

			Fade::GetInstance()->StartFadeOut();

			isFade_ = true;
		}
	}
}

void GameOverEvent::Draw() {
}

GameSceneEventBase::EventType GameOverEvent::RequestNextEvent() const {

	return EventType::NONE;
}
#include "GameOverEvent.h"

#include <UIManager.h>
#include <Shake/Shake.h>
#include <Fade/Fade.h>
#include <Base/OffScreen.h>
#include <Base/Input.h>

void GameOverEvent::Start(Player* player, Camera* camera) {

	changeScene_ = true;

	//スプライトを映す
	UIManager::GetInstance()->GetUIGroup("GameOver")->isActive = true;

	//テキスト系のUIを隠す
	UIManager::GetInstance()->GetUIGroup("Reticle")->isActive = false;

	UIManager::GetInstance()->GetUIGroup("Help")->isActive = false;

	//シェイクを始める
	Shake::GetInstance()->Start(1.0f, 0.5f);

	//色を反転させる
	OffScreen::GetInstance()->SetColorReverseRatio(1.0f);
}

void GameOverEvent::Exit() {
}

void GameOverEvent::Update() {

	if (Input::GetInstance()->IsTriggerPushKey(DIK_SPACE)) {

		if (!isFade_) {

			Fade::GetInstance()->StartFadeOut();

			isFade_ = true;
		}
	}
}

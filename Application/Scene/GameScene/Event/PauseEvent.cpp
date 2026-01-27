#include "PauseEvent.h"

#include <UIManager.h>
#include <EmitterManager.h>
#include <Fade/Fade.h>
#include <Base/Input.h>

void PauseEvent::Start(Player* player, Camera* camera) {

	player_ = player;
	camera_ = camera;

	canMove_ = false;

	changeScene_ = true;

	UIManager::GetInstance()->GetUIGroup("Reticle")->isActive = false;
	UIManager::GetInstance()->GetUIGroup("Help")->isActive = false;
	UIManager::GetInstance()->GetUIGroup("Pause")->isActive = true;

	EmitterManager::GetInstance()->SetIsUpdate(false);

	player_->SetIsMoveActive(false);
}

void PauseEvent::Exit() {

	UIManager::GetInstance()->GetUIGroup("Reticle")->isActive = true;
	UIManager::GetInstance()->GetUIGroup("Help")->isActive = true;
	UIManager::GetInstance()->GetUIGroup("Pause")->isActive = false;

	EmitterManager::GetInstance()->SetIsUpdate(true);

	player_->SetIsMoveActive(true);
}

void PauseEvent::Update() {

	if (delay_) {

		if (Input::GetInstance()->IsTriggerPushKey(DIK_SPACE)) {

			isFinished_ = true;
		}

		if (Input::GetInstance()->IsTriggerPushKey(DIK_ESCAPE)) {

			Fade::GetInstance()->StartFadeOut();
		}
	}

	delay_ = true;
}
#include "GameEvent.h"

void GameEvent::Start(Player* player, Camera* camera) {

	player_ = player;
	camera_ = camera;

	canMove_ = true;

	changeScene_ = false;

	followCamera_->SetIsActive(true);

	player_->SetIsMoveActive(true);
}

void GameEvent::Exit() {
}

void GameEvent::Update() {
}

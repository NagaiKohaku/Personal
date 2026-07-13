#include "TankMoveState.h"

#include <Object/Player/Player.h>

#include <Math/Utility/Easing.h>

#include <algorithm>
#include <numbers>

using namespace MyEngine;

void TankMoveState::Enter() {

	rotRange_ = { 0.5f,0.0f,0.3f };

	leftWingTranslate_ = { -0.75f, 0.0f, 0.0f };

	leftWingRotate_ = { 0.0f, 0.0f, -std::numbers::pi_v<float> / 2.0f - std::numbers::pi_v<float> *2.0f };

	leftWingTrailPos_ = { 0.75f, 0.0f, 0.0f };

	rightWingTranslate_ = { 0.75f, 0.0f, 0.0f };

	rightWingRotate_ = { 0.0f, 0.0f, std::numbers::pi_v<float> / 2.0f + std::numbers::pi_v<float> *2.0f };

	rightWingTrailPos_ = { -0.75f, 0.0f, 0.0f };
}

void TankMoveState::Update(Player* player) {

	//高度が一定値を超えたらジェットモードに変更
	if (player->GetWorldPos().y > stateChangeHeight_) {

		player->ChangeJetState();

		return;
	}

	MovementState::Update(player);
}
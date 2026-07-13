#include "JetMoveState.h"

#include <Object/Player/Player.h>

#include <Math/Utility/Easing.h>

#include <algorithm>

using namespace MyEngine;

void JetMoveState::Enter() {

	rotRange_ = { 0.5f,0.0f,0.3f };

	leftWingTranslate_ = { -1.35f, 0.0f, 0.0f };

	leftWingRotate_ = { 0.0f, 0.0f, 0.0f };

	leftWingTrailPos_ = { -0.75f, 0.0f, 0.0f };

	rightWingTranslate_ = { 1.35f, 0.0f, 0.0f };

	rightWingRotate_ = { 0.0f, 0.0f, 0.0f };

	rightWingTrailPos_ = { 0.75f, 0.0f, 0.0f };
}

void JetMoveState::Update(Player* player) {

	//高度が一定値を下回ったら戦車モードに変更
	if (player->GetWorldPos().y <= stateChangeHeight_) {

		player->ChangeTankState();

		return;
	}

	MovementState::Update(player);
}
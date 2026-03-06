#include "GameEvent.h"

#include <Base/Input.h>
#include <2d/Sprite/Sprite.h>
#include <Object/Manager/ObjectManager.h>
#include <Effect/Manager/UIManager.h>
#include <Math/Utility/MakeMatrixMath.h>

using namespace MyEngine;

void GameEvent::Start(EngineContext context, Player* player, FollowCamera* followCamera) {

	context_ = context;
	player_ = player;
	followCamera_ = followCamera;

	canMove_ = true;

	UIManager::GetInstance()->GetUIGroup("Reticle")->isActive = true;

	followCamera_->SetIsActive(true);

	player_->SetIsMoveActive(true);
}

void GameEvent::Exit() {
}

void GameEvent::Update() {

	//3Dオブジェクトの座標をスクリーン座標に変換する
	Vector3 playerScreenPos = Vector3ToScreenSpace(context_.camera, player_->GetWorldPos());

	UIManager::GetInstance()->GetUIGroup("Reticle")->transform.translate_ = playerScreenPos;

	if (context_.input->isPushKey(DIK_W)) {

		UIManager::GetInstance()->Get2DObject("Reticle", "WButton")->GetSprite()->SetRatio(1.0f);
	} else {

		UIManager::GetInstance()->Get2DObject("Reticle", "WButton")->GetSprite()->SetRatio(0.0f);
	}

	if (context_.input->isPushKey(DIK_A)) {

		UIManager::GetInstance()->Get2DObject("Reticle", "AButton")->GetSprite()->SetRatio(1.0f);
	} else {

		UIManager::GetInstance()->Get2DObject("Reticle", "AButton")->GetSprite()->SetRatio(0.0f);
	}

	if (context_.input->isPushKey(DIK_S)) {

		UIManager::GetInstance()->Get2DObject("Reticle", "SButton")->GetSprite()->SetRatio(1.0f);
	} else {

		UIManager::GetInstance()->Get2DObject("Reticle", "SButton")->GetSprite()->SetRatio(0.0f);
	}

	if (context_.input->isPushKey(DIK_D)) {

		UIManager::GetInstance()->Get2DObject("Reticle", "DButton")->GetSprite()->SetRatio(1.0f);
	} else {

		UIManager::GetInstance()->Get2DObject("Reticle", "DButton")->GetSprite()->SetRatio(0.0f);
	}

	UIManager::GetInstance()->Get2DObject("Reticle", "SpaceButton")->GetSprite()->SetRatio(player_->GetAttackTimeRatio());
}

void GameEvent::Draw() {
}

GameSceneEventBase::EventType GameEvent::RequestNextEvent() const {

	if (context_.input->IsTriggerPushKey(DIK_ESCAPE)) {

		return EventType::PAUSE;
	}

	if (ObjectManager::GetInstance()->GetKillCount() >= 30) {

		return EventType::CLEAR;
	}

	if (player_->GetIsDead()) {

		return EventType::GAMEOVER;
	}

	return EventType::NONE;
}

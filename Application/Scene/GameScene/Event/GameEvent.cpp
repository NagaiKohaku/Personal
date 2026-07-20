#include "GameEvent.h"

#include <Base/Input.h>
#include <Object/Manager/ObjectManager.h>
#include <UI/UIManager.h>
#include <Math/Utility/MakeMatrixMath.h>

using namespace MyEngine;

void GameEvent::Start(MyEngine::EngineContext context, Player* player, FollowCamera* followCamera) {

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

	if (Input::GetInstance()->isPushKey(DIK_W)) {

		UIManager::GetInstance()->Get2DObject("Reticle", "WButton")->GetSprite()->SetRatio(1.0f);
	} else {

		UIManager::GetInstance()->Get2DObject("Reticle", "WButton")->GetSprite()->SetRatio(0.0f);
	}

	if (Input::GetInstance()->isPushKey(DIK_A)) {

		UIManager::GetInstance()->Get2DObject("Reticle", "AButton")->GetSprite()->SetRatio(1.0f);
	} else {

		UIManager::GetInstance()->Get2DObject("Reticle", "AButton")->GetSprite()->SetRatio(0.0f);
	}

	if (Input::GetInstance()->isPushKey(DIK_S)) {

		UIManager::GetInstance()->Get2DObject("Reticle", "SButton")->GetSprite()->SetRatio(1.0f);
	} else {

		UIManager::GetInstance()->Get2DObject("Reticle", "SButton")->GetSprite()->SetRatio(0.0f);
	}

	if (Input::GetInstance()->isPushKey(DIK_D)) {

		UIManager::GetInstance()->Get2DObject("Reticle", "DButton")->GetSprite()->SetRatio(1.0f);
	} else {

		UIManager::GetInstance()->Get2DObject("Reticle", "DButton")->GetSprite()->SetRatio(0.0f);
	}

	UIManager::GetInstance()->Get2DObject("Reticle", "SpaceButton")->GetSprite()->SetRatio(player_->GetAttackTimeRatio());

	std::vector<Object2D*> hpUIs;

	int playerHP = player_->GetHP();

	hpUIs.push_back(UIManager::GetInstance()->Get2DObject("Reticle", "HP01"));
	hpUIs.push_back(UIManager::GetInstance()->Get2DObject("Reticle", "HP02"));
	hpUIs.push_back(UIManager::GetInstance()->Get2DObject("Reticle", "HP03"));

	for (auto hpUI : hpUIs) {

		if (playerHP > 0) {

			hpUI->GetSprite()->SetRatio(1.0f);
		} else {

			hpUI->GetSprite()->SetRatio(0.0f);
		}

		playerHP--;
	}
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

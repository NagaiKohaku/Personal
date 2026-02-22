#include "StartEvent.h"

#include <numbers>

using namespace MyEngine;

void StartEvent::Start(MyEngine::EngineContext context, Player* player, FollowCamera* followCamera) {

	context_ = context;
	player_ = player;
	followCamera_ = followCamera;

	motionNum_ = 1;

	canMove_ = true;

	motionPoint_.push_back({ Vector3(0.0f,20.0f,-600.0f),Vector3(0.0f,-std::numbers::pi_v<float>,0.0f),0.0f,EaseType::LINEAR,1.0f });
	motionPoint_.push_back({ Vector3(0.0f,20.0f,-600.0f),Vector3(0.0f,-std::numbers::pi_v<float>,0.0f),1.0f,EaseType::EASE_OUT,1.0f });
	motionPoint_.push_back({ Vector3(0.0f,0.75f,0.0f),Vector3(0.2f,-std::numbers::pi_v<float>,0.0f),2.0f,EaseType::EASE_OUT,3.0f });
	motionPoint_.push_back({ Vector3(0.0f,1.0f,0.0f),Vector3(0.2f,0.0f,0.0f),4.0f,EaseType::EASE_OUT,4.0f });

	followCamera_->SetIsActive(false);

	player_->SetIsMoveActive(false);
}

void StartEvent::Exit() {
}

void StartEvent::Update() {

	UpdateEventMotion();
}

void StartEvent::Draw() {
}

GameSceneEventBase::EventType StartEvent::RequestNextEvent() const {

	if (isFinished_) {

		return EventType::GAME;
	}

	return EventType::NONE;
}

#pragma once

#include <Scene/SceneEvent.h>

#include <Camera/FollowCamera.h>

class GameEvent : public SceneEvent {

public:

	void Start(Player* player, Camera* camera) override;

	void Exit() override;

	void Update() override;

private:

	FollowCamera* followCamera_ = nullptr;

public:

	void SetFollowCamera(FollowCamera* followCamera) { followCamera_ = followCamera; }

};
#pragma once

#include <Scene/SceneEvent.h>

class ClearEvent : public SceneEvent {

public:

	void Start(Player* player, Camera* camera) override;

	void Exit() override;

	void Update() override;

private:

	bool isFade_ = false;
};
#pragma once

#include <Scene/GameScene/Event/GameSceneEventBase.h>
#include <Player/Player.h>
#include <3d/Camera/Camera.h>
#include <Camera/FollowCamera.h>

#include <memory>

class GameSceneEventFactory {

public:

	static std::unique_ptr<GameSceneEventBase> Create(GameSceneEventBase::EventType type);

};
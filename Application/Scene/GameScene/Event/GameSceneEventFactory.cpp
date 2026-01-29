#include "GameSceneEventFactory.h"

#include <Scene/GameScene/Event/StartEvent.h>
#include <Scene/GameScene/Event/GameEvent.h>
#include <Scene/GameScene/Event/PauseEvent.h>
#include <Scene/GameScene/Event/ClearEvent.h>
#include <Scene/GameScene/Event/GameOverEvent.h>

std::unique_ptr<GameSceneEventBase> GameSceneEventFactory::Create(GameSceneEventBase::EventType type) {

	switch (type) {
	case GameSceneEventBase::EventType::START:

		return std::make_unique<StartEvent>();
	case GameSceneEventBase::EventType::GAME:

		return std::make_unique<GameEvent>();
	case GameSceneEventBase::EventType::PAUSE:

		return std::make_unique<PauseEvent>();
	case GameSceneEventBase::EventType::CLEAR:

		return std::make_unique<ClearEvent>();
	case GameSceneEventBase::EventType::GAMEOVER:

		return std::make_unique<GameOverEvent>();
	default:

		return nullptr;
	}
}
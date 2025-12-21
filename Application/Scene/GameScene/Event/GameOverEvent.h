#pragma once

#include <Scene/GameScene/GameScene3DObject.h>
#include <Scene/GameScene/GameScene2DObject.h>

class GameOverEvent {

public:

	void Initialize(GameScene3DObject* object3D, GameScene2DObject* object2D);

	void Update();

private:

	GameScene3DObject* object3D_;

	GameScene2DObject* object2D_;

	bool isGameOver_;

};
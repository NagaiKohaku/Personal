#pragma once

#include <Scene/GameScene/GameScene3DObject.h>
#include <Scene/GameScene/GameScene2DObject.h>
#include <AnimPoint.h>

#include <Player/Player.h>
#include <3d/Camera/Camera.h>
#include <Camera/FollowCamera.h>

class StartEvent {

public:

	void Initialize(GameScene3DObject* object3D, GameScene2DObject* object2D);

	void Update();

	void Animation();

private:

	GameScene3DObject* object3D_;

	GameScene2DObject* object2D_;

	//スタートアニメーションキーフレーム
	std::vector<AnimPoint> startAnimPoints_;

	//アニメーションのタイマー
	float timer_;

	//キーフレームの番号
	int animNum_;

	bool isStart_;

	bool isAnimation_;

};


#pragma once

#include <Scene/GameScene/GameScene3DObject.h>
#include <Scene/GameScene/GameScene2DObject.h>
#include <AnimPoint.h>

class ClearEvent {

public:

	void Initialize(GameScene3DObject* object3D, GameScene2DObject* object2D);

	void Update();

	void Animation();

private:

	GameScene3DObject* object3D_;

	GameScene2DObject* object2D_;

	//クリアアニメーションキーフレーム
	std::vector<AnimPoint> clearAnimPoints_;

	uint32_t killCountMax_;

	//アニメーションのタイマー
	float timer_;

	//キーフレームの番号
	int animNum_;

	bool isClear_;

	bool isAnimation_;

	bool switchSpriteHide_;

	bool switchSpriteShow_;

public:

	bool GetIsClear() { return isClear_; }
}
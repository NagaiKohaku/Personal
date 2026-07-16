#pragma once

#include "2d/Object/Object2D.h"
#include "3d/Camera/Camera.h"
#include "Math/Transform/WorldTransform.h"

#include <memory>
#include <vector>

class CountNumber {

public:

	void Initialize(MyEngine::Camera* _camera);

	void Update();

	void Draw();

private:

	void CreateOnesPlaceSprite(std::string fileName);

	void CreateTensPlaceSprite(std::string fileName);

private:

	MyEngine::Camera* camera_;

	std::unique_ptr<MyEngine::WorldTransform> worldTransform_;

	std::vector<std::unique_ptr<MyEngine::Object2D>> onesPlaceSprites_;

	std::vector<std::unique_ptr<MyEngine::Object2D>> tensPlaceSprites_;

	int drawNumber_ = 0;

	MyEngine::Vector2 centerPos_ = { 640.0f,360.0f };

	MyEngine::Vector2 distance_ = { 27.5f,0.0f };

	std::vector<std::string> fileNames_ = {
		"NumberZero",
		"NumberOne",
		"NumberTwo",
		"NumberThree",
		"NumberFour",
		"NumberFive",
		"NumberSix",
		"NumberSeven",
		"NumberEight",
		"NumberNine"
	};

	bool isDraw_ = true;

public:

	void SetCenterPos(const MyEngine::Vector2 pos) { centerPos_ = pos; }

	void SetDrawNumber(const int number) { drawNumber_ = number; }

	void SetIsDraw(const bool flag) { isDraw_ = flag; }
};
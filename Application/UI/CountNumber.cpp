#include "CountNumber.h"

using namespace MyEngine;

void CountNumber::Initialize(Camera* _camera) {

	camera_ = _camera;

	for (auto& fileName : fileNames_) {

		CreateOnesPlaceSprite(fileName);
		CreateTensPlaceSprite(fileName);
	}

}

void CountNumber::Update() {

	for (auto& onesSprite : onesPlaceSprites_) {

		onesSprite->SetTranslate(centerPos_ - distance_);

		onesSprite->Update();
	}

	for (auto& tensSprite : tensPlaceSprites_) {

		tensSprite->SetTranslate(centerPos_ + distance_);

		tensSprite->Update();
	}
}

void CountNumber::Draw() {

	int onesPlace = drawNumber_ / 10;

	int tensPlace = drawNumber_ % 10;

	if (isDraw_) {

		onesPlaceSprites_[onesPlace]->Draw(LayerType::UI);

		tensPlaceSprites_[tensPlace]->Draw(LayerType::UI);
	}
}

void CountNumber::CreateOnesPlaceSprite(std::string fileName) {

	std::unique_ptr<Object2D> newObject = std::make_unique<Object2D>();

	newObject->Initialize(camera_);

	newObject->SetSprite(fileName);

	onesPlaceSprites_.push_back(std::move(newObject));
}

void CountNumber::CreateTensPlaceSprite(std::string fileName) {

	std::unique_ptr<Object2D> newObject = std::make_unique<Object2D>();

	newObject->Initialize(camera_);

	newObject->SetSprite(fileName);

	tensPlaceSprites_.push_back(std::move(newObject));
}
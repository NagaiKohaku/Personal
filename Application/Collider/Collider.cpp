#include "Collider.h"

void Collider::Initialize(WorldTransform& parent) {

	worldTransform_.Initialize();

	worldTransform_.SetParent(&parent);

	isDraw_ = true;

	isActive_ = true;

	defaultColor_ = { 0.0f,1.0f,0.0f,1.0f };

	hitColor_ = { 1.0f,0.0f,0.0f,1.0f };

	CreateDebugLines();
}

void Collider::Update() {

	worldTransform_.UpdateMatrix();

	for (auto& line : debugLines_) {
		
		line->Update();
	}
}

void Collider::Draw() {

	if (!isDraw_) {
		return;
	}

	for (auto& line : debugLines_) {
		line->Draw();
	}
}

void Collider::CreateDebugLines() {
}
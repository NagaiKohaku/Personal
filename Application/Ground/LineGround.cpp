#include "LineGround.h"

void LineGround::Initialize() {

	/// === 床のラインを生成 === ///

	lineDivide_ = 30.0f;

	lineDistance_ = 10.0f;

	for (size_t i = 0; i < lineDivide_ + 1; i++) {

		std::unique_ptr<DebugLine> newLine = std::make_unique<DebugLine>();

		newLine->Initialize(
			{ (i - lineDivide_ / 2.0f) * lineDistance_, 0.0f, (-lineDivide_ / 2.0f) * lineDistance_ },
			{ (i - lineDivide_ / 2.0f) * lineDistance_, 0.0f, (lineDivide_ / 2.0f) * lineDistance_ },
			{ 1.0f,1.0f,1.0f,1.0f }
		);

		if (i == static_cast<int>(lineDivide_ / 2.0f)) {
			newLine->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		}

		verticalLines_.push_back(std::move(newLine));
	}

	for (size_t i = 0; i < lineDivide_ + 1; i++) {

		std::unique_ptr<DebugLine> newLine = std::make_unique<DebugLine>();

		newLine->Initialize(
			{ (-lineDivide_ / 2.0f) * lineDistance_, 0.0f, (i - lineDivide_ / 2.0f) * lineDistance_ },
			{ (lineDivide_ / 2.0f) * lineDistance_, 0.0f, (i - lineDivide_ / 2.0f) * lineDistance_ },
			{ 1.0f,1.0f,1.0f,1.0f }
		);

		horizontalLines_.push_back(std::move(newLine));
	}

	speed_ = 1.0f;

}

void LineGround::Update() {

	Move();

	for (auto& line : verticalLines_) {

		line->Update();
	}

	for (auto& line : horizontalLines_) {

		line->Update();
	}
}

void LineGround::Draw() {

	for (auto& line : verticalLines_) {

		line->Draw(LayerType::Object);
	}

	for (auto& line : horizontalLines_) {

		line->Draw(LayerType::Object);
	}
}

void LineGround::Move() {

	for (auto& line : horizontalLines_) {

		line->GetWorldTransform().translate_.z -= speed_;

		if (line->GetWorldTransform().translate_.z <= (0.0f - lineDivide_ / 2.0f) * lineDistance_) {

			line->GetWorldTransform().translate_.z = (lineDivide_ - lineDivide_ / 2.0f) * lineDistance_;
		}
	}
}
#include "LineGround.h"

using namespace MyEngine;

///=====================================================/// 
/// ライン状の床オブジェクトを初期化
///=====================================================///
void LineGround::Initialize() {

	/// === 床のラインを生成 === ///

	//パラメータの設定
	lineDivide_ = 30.0f;

	lineDistance_ = 10.0f;

	speed_ = 1.0f;

	//縦ラインの生成
	for (size_t i = 0; i < lineDivide_ + 1; i++) {

		std::unique_ptr<DebugLine> newLine = std::make_unique<DebugLine>();

		newLine->Initialize(
			{ (i - lineDivide_ / 2.0f) * lineDistance_, 0.0f, (-lineDivide_ / 2.0f) * lineDistance_ },
			{ (i - lineDivide_ / 2.0f) * lineDistance_, 0.0f, (lineDivide_ / 2.0f) * lineDistance_ },
			{ 1.0f,1.0f,1.0f,1.0f }
		);

		//中央ラインは赤色に変更
		if (i == static_cast<int>(lineDivide_ / 2.0f)) {
			newLine->SetColor({ 1.0f,0.0f,0.0f,1.0f });
		}

		verticalLines_.push_back(std::move(newLine));
	}

	//横ラインの生成
	for (size_t i = 0; i < lineDivide_ + 1; i++) {

		std::unique_ptr<DebugLine> newLine = std::make_unique<DebugLine>();

		newLine->Initialize(
			{ (-lineDivide_ / 2.0f) * lineDistance_, 0.0f, (i - lineDivide_ / 2.0f) * lineDistance_ },
			{ (lineDivide_ / 2.0f) * lineDistance_, 0.0f, (i - lineDivide_ / 2.0f) * lineDistance_ },
			{ 1.0f,1.0f,1.0f,1.0f }
		);

		horizontalLines_.push_back(std::move(newLine));
	}
}

///=====================================================/// 
/// ライン状の床オブジェクトの更新処理
///=====================================================///
void LineGround::Update() {

	//移動処理
	Move();

	//縦ライン更新
	for (auto& line : verticalLines_) {

		line->Update();
	}

	//横ライン更新
	for (auto& line : horizontalLines_) {

		line->Update();
	}
}

///=====================================================/// 
/// ライン状の床オブジェクトを描画
///=====================================================///
void LineGround::Draw() {

	//縦ライン描画
	for (auto& line : verticalLines_) {

		line->Draw(LayerType::OBJECT);
	}

	//横ライン描画
	for (auto& line : horizontalLines_) {

		line->Draw(LayerType::OBJECT);
	}
}

///=====================================================/// 
/// ライン状の床オブジェクトの横ラインを移動
///=====================================================///
void LineGround::Move() {

	//横ラインのみ移動
	for (auto& line : horizontalLines_) {

		//Z座標を移動
		line->GetWorldTransform().translate_.z -= speed_;

		//範囲外に出たら位置リセット
		if (line->GetWorldTransform().translate_.z <= (0.0f - lineDivide_ / 2.0f) * lineDistance_) {

			line->GetWorldTransform().translate_.z = (lineDivide_ - lineDivide_ / 2.0f) * lineDistance_;
		}
	}
}
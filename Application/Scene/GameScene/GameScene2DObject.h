#pragma once

#include <2d/Object/Object2D.h>

#include <memory>

class GameScene2DObject {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	void Initialize();

	void Update();

	void Draw();

	void HideSprite();

	void ShowSprite();

private:

	//ゲームオーバースプライト
	std::unique_ptr<Object2D> gameOverSprite_;

	//ゲームオーバースペーススプライト
	std::unique_ptr<Object2D> gameOverSpaceSprite_;

	//ゲームオーバー左矢印スプライト
	std::unique_ptr<Object2D> gameOverLeftArrowSprite_;

	//ゲームオーバー右矢印スプライト
	std::unique_ptr<Object2D> gameOverRightArrowSprite_;

	//ゲームクリアスプライト
	std::unique_ptr<Object2D> gameClearSprite_;

	//ゲームクリアスペーススプライト
	std::unique_ptr<Object2D> gameClearSpaceSprite_;

	//ゲームクリア左矢印スプライト
	std::unique_ptr<Object2D> gameClearLeftArrowSprite_;

	//ゲームクリア右矢印スプライト
	std::unique_ptr<Object2D> gameClearRightArrowSprite_;

	//ヘルプスプライト
	std::unique_ptr<Object2D> helpSprite_;

	//スペースキースプライトの位置
	Vector2 spaceKeyPos_;

	//スペースキースプライトの大きさ
	Vector2 spaceKeySize_;

	//矢印の長さ
	float arrowLength_;

	//矢印のタイマー
	float arrowTimer_;

	//アニメーションタイマーの進行方向
	float timerDirection_;

	//描画フラグ
	bool isDraw_;

	bool isClear_;

	bool isGameOver_;
};


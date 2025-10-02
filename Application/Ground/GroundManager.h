#pragma once

#include <Ground/Ground.h>
#include <Ground/Building.h>

#include "memory"
#include "vector"

class GroundManager {

public:

	// 初期化
	void Initialize();

	// 終了処理
	void Finalize();

	// 更新
	void Update();

	// 描画
	void Draw();

	void ImGui();

private:

	//地面
	std::unique_ptr<Ground> ground_;

	// 建物
	std::vector<std::unique_ptr<Building>> building_;

	float radius_;

	float height_;

	float distance_;

	float startPosX_;

	float startPosZ_;

};
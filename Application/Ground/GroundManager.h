#pragma once

#include "memory"
#include "vector"

class Building;

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

	// 建物
	std::vector<std::unique_ptr<Building>> building_;

	float radius_;

	float height_;

	float distance_;

	float startPosX_;

	float startPosZ_;

};
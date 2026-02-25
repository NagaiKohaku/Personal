#pragma once

#include <Object/Ground/Ground.h>
#include <Object/Ground/Building.h>

#include "memory"
#include "vector"

class GroundManager {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 地面と建物群を初期化します。
	/// </summary>
	void Initialize(MyEngine::Object3DCommon* object3DCommonPtr, MyEngine::DebugObjectCommon* debugObjectCommonPtr);

	/// <summary>
	/// 地面と建物群の後片付けを行います。
	/// </summary>
	void Finalize();

	/// <summary>
	/// 地面と建物群の更新処理を行います。
	/// </summary>
	void Update();

	/// <summary>
	/// 地面と建物群の座標更新を行います。
	/// </summary>
	void TransformUpdate();

	/// <summary>
	/// 地面と建物群を描画します。
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGuiによるパラメータ調整
	/// </summary>
	void ImGui();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//地面オブジェクト
	std::unique_ptr<Ground> ground_;

	//建物オブジェクト
	std::vector<std::unique_ptr<Building>> building_;

	//建物の幅
	float buildingWidth_;

	//建物の高さ
	float buildingHeight_;

	//建物のランダム範囲
	float buildingRandomRange_;

	//建物同士の距離
	float buildingDistance_;

	//建物の開始位置X
	float buildingStartX_;

	//建物の開始位置Z
	float buildingStartZ_;

	//建物の最大数
	int maxBuildingNum_;

};
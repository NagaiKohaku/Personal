#pragma once

#include <Ground/Ground.h>
#include <Ground/Building.h>

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
	/// <remarks>
	/// - 建物の幅、高さ、生成範囲、距離、開始位置、最大数などのパラメータを設定します。
	/// - 地面オブジェクトを生成し、初期化します。
	/// - 建物を行・列に沿って生成し、ランダムな高さを付与します。
	/// - 偶数列の建物はX座標を反転させ、左右対称に配置します。
	/// - 生成した建物は内部コンテナに保持されます。
	/// - Z方向の配置が開始位置を超えた場合、建物の生成を停止します。
	/// </remarks>
	void Initialize();

	/// <summary>
	/// 地面と建物群の後片付けを行います。
	/// </summary>
	/// <remarks>
	/// - 内部で保持している建物オブジェクトのコンテナをクリアし、メモリを解放します。
	/// - 地面オブジェクト自体はスマートポインタにより自動的に破棄されます。
	/// </remarks>
	void Finalize();

	/// <summary>
	/// 地面と建物群の更新処理を行います。
	/// </summary>
	/// <remarks>
	/// - 地面オブジェクトの更新処理を呼び出します。
	/// - 各建物オブジェクトを更新し、Z座標が開始位置を超えた場合はリセットします。
	/// - 建物の移動やワールド変換が反映されます。
	/// </remarks>
	void Update();

	/// <summary>
	/// 地面と建物群の座標更新を行います。
	/// </summary>
	/// <remarks>
	/// - 地面オブジェクトの座標更新を呼び出します。
	/// - 各建物オブジェクトの座標更新を呼び出し、ワールド変換を反映させます。
	/// </remarks>
	void TransformUpdate();

	/// <summary>
	/// 地面と建物群を描画します。
	/// </summary>
	/// <remarks>
	/// - 地面オブジェクトを描画します。
	/// - 各建物オブジェクトを描画します。
	/// - 描画はそれぞれの内部オブジェクトの描画処理を通じて行われます。
	/// </remarks>
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
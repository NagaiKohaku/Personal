#pragma once

#include "Base/LayerType.h"

#include "functional"
#include "map"
#include "vector"

///=====================================================/// 
/// 描画管理クラス
///=====================================================///
class Renderer {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 静的インスタンス取得
	/// </summary>
	/// <returns>静的インスタンス</returns>
	static Renderer* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画コマンド追加
	/// </summary>
	/// <param name="layer">レイヤー</param>
	/// <param name="func">描画コマンド</param>
	void AddDraw(LayerType layer,std::function<void()> func);

	///-------------------------------------------/// 
	/// クラス内関数
	///-------------------------------------------///
private:

	/// <summary>
	/// キューの初期化
	/// </summary>
	void ClearQueue();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//描画コマンドキュー
	std::map<LayerType, std::vector<std::function<void()>>> drawQueue_;
};
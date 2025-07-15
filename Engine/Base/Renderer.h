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
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static Renderer* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// スワップチェーン描画
	/// </summary>
	void SwapChainDraw();

	/// <summary>
	/// オフスクリーン描画
	/// </summary>
	void OffScreenDraw();

	/// <summary>
	/// 描画コマンド追加
	/// </summary>
	/// <param name="layer">レイヤー</param>
	/// <param name="isOffScreen">オフスクリーンに送るか</param>
	/// <param name="func">コマンド</param>
	void AddDraw(LayerType layer, bool isOffScreen,std::function<void()> func);

	///-------------------------------------------/// 
	/// クラス内関数
	///-------------------------------------------///
private:

	/// <summary>
	/// キューの初期化
	/// </summary>
	void ClearQueue();

	/// <summary>
	/// オフスクリーンキューの初期化
	/// </summary>
	void ClearOffScreenQueue();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//描画コマンドキュー
	std::map<LayerType, std::vector<std::function<void()>>> drawQueue_;

	//OffScreen用の描画コマンドキュー
	std::map<LayerType, std::vector<std::function<void()>>> offScreenDrawQueue_;
};
#pragma once

#include "2d/Object/Object2D.h"

/// <summary>
/// 画面全体にフラッシュエフェクトを表示する管理クラスです。
/// </summary>
class Flash {

public:

	/// <summary>
	/// Flashのシングルトンインスタンスを取得します。
	/// </summary>
	static Flash* GetInstance();

	/// <summary>
	/// フラッシュエフェクトの初期化を行います。
	/// </summary>
	void Initialize();

	/// <summary>
	/// フラッシュエフェクトの更新処理を行います。
	/// </summary>
	void Update();

	/// <summary>
	/// フラッシュエフェクトを描画します。
	/// </summary>
	void Draw();

	/// <summary>
	/// フラッシュエフェクトを開始します。
	/// </summary>
	/// <param name="maxTime">フラッシュが表示される最大時間（秒単位）です。</param>
	/// <param name="color">フラッシュの色をRGBAで指定します。</param>
	void Start(float maxTime, MyEngine::Vector4 color);

private:

	//フラッシュ用オブジェクト
	std::unique_ptr<MyEngine::Object2D> flashObject_;

	//アルファ値
	float alpha_;

	//フラッシュ中かどうか
	bool isFlash_;

	//タイマー
	float timer_;

	//最大時間
	float maxTime_;

	//フラッシュ色
	MyEngine::Vector4 color_;
};
#pragma once

#include "2d/Object/Object2D.h"

/// <summary>
/// 画面全体にフラッシュエフェクトを表示する管理クラスです。
/// </summary>
/// <remarks>
/// - シングルトンとして管理され、ゲーム内の任意のタイミングでフラッシュを開始できます。
/// - 内部でフラッシュ用の2Dオブジェクトを保持し、更新・描画処理を行います。
/// - フラッシュの色や表示時間を柔軟に指定可能です。
/// </remarks>
class Flash {

public:

	/// <summary>
	/// Flashのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static Flash* GetInstance();

	/// <summary>
	/// フラッシュエフェクトの初期化を行います。
	/// </summary>
	/// <remarks>
	/// - フラッシュ用のスプライトを読み込みます。
	/// - フラッシュ表示用の2Dオブジェクトを生成し、初期化します。
	/// - オブジェクトの位置、サイズ、スプライトのアンカーポイントおよび色を設定します。
	/// - フラッシュの透明度、実行フラグ、タイマー、最大表示時間、表示色を初期状態に設定します。
	/// </remarks>
	void Initialize();

	/// <summary>
	/// フラッシュエフェクトの更新処理を行います。
	/// </summary>
	/// <remarks>
	/// - フラッシュ中であればタイマーを更新し、透明度を計算します。
	/// - 透明度が0以下になった場合はフラッシュを終了します。
	/// - スプライトの色に現在の透明度を反映させます。
	/// - フラッシュ用オブジェクトの内部更新処理を呼び出します。
	/// </remarks>
	void Update();

	/// <summary>
	/// フラッシュエフェクトを描画します。
	/// </summary>
	/// <remarks>
	/// - フラッシュ用の2DオブジェクトをUIレイヤーに描画します。
	/// </remarks>
	void Draw();

	/// <summary>
	/// フラッシュエフェクトを開始します。
	/// </summary>
	/// <param name="maxTime">フラッシュが表示される最大時間（秒単位）です。</param>
	/// <param name="color">フラッシュの色をRGBAで指定します。</param>
	/// <remarks>
	/// - 透明度とタイマーを初期化します。
	/// - 最大表示時間を設定します。
	/// - フラッシュ中フラグを有効にします。
	/// - 指定された色をフラッシュ色として設定します。
	/// </remarks>
	void Start(float maxTime, Vector4 color);

private:

	//フラッシュ用オブジェクト
	std::unique_ptr<Object2D> flashObject_;

	//アルファ値
	float alpha_;

	//フラッシュ中かどうか
	bool isFlash_;

	//タイマー
	float timer_;

	//最大時間
	float maxTime_;

	//フラッシュ色
	Vector4 color_;
};
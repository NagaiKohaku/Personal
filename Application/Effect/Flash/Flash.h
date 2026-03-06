#pragma once

#include <Object/2D/Object2D.h>
#include <Object/2D/Object2DCommon.h>
#include <3d/Camera/Camera.h>
#include <Base/Renderer.h>

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
	void Initialize(MyEngine::Object2DCommon* object2DCommonPtr, MyEngine::Camera* cameraPtr, MyEngine::Renderer* rendererPtr);

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

	//カメラ
	MyEngine::Camera* camera_;

	MyEngine::Renderer* renderer_;

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
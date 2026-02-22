#pragma once

#include <Base/WinApp.h>
#include <2d/Object/Object2D.h>
#include <Math/Transform/WorldTransform.h>

#include <3d/Camera/Camera.h>
#include <Object/Player/Player.h>

#include <memory>
#include <vector>

/// <summary>
/// フェード演出を管理するクラスです。
/// </summary>
class Fade {

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
public:

	//フェードの状態
	enum class FadeState {
		NONE,
		FADE_IN,
		FADE_IN_END,
		FADE_OUT,
		FADE_OUT_END
	};

	//フェード用にスプライトの情報をまとめた構造体
	struct FadeSprite {
		std::unique_ptr<MyEngine::Object2D> sprite;
		MyEngine::Vector2 startSize;
		MyEngine::Vector2 endSize;
		float startTime;
		float endTime;
	};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// Fadeのシングルトンインスタンスを取得します。
	/// </summary>
	static Fade* GetInstance();

	/// <summary>
	/// フェード処理に必要なスプライトやリソースを初期化する関数です。
	/// </summary>
	void Initialize(MyEngine::WinApp* winAppPtr, MyEngine::Camera* cameraPtr);

	/// <summary>
	/// フェード処理の状態を毎フレーム更新する関数です。
	/// </summary>
	void Update();

	/// <summary>
	/// 管理しているフェード用スプライトを画面に描画する関数です。
	/// </summary>
	void Draw();

	/// <summary>
	/// フェードイン処理を開始する関数です。
	/// </summary>
	void StartFadeIn();

	/// <summary>
	/// フェードインの進行処理を行います。
	/// </summary>
	void FadeInUpdate();

	/// <summary>
	/// フェードアウト処理を開始する関数です。
	/// </summary>
	void StartFadeOut();

	/// <summary>
	/// フェードアウトの進行処理を行います。
	/// </summary>
	void FadeOutUpdate();

	///=====================================================/// 
	/// クラス内関数
	///=====================================================///
private:

	/// <summary>
	/// リング形状のスプライトを生成し、フェード用の管理リストに追加します。
	/// </summary>
	void CreateRingSprite();

	/// <summary>
	/// 円形のスプライトを生成し、フェード用の管理リストに追加します。
	/// </summary>
	void CreateCircleSprite();

	///=====================================================/// 
	/// メンバ変数
	///=====================================================///
private:

	//ウィンドウクラス
	MyEngine::WinApp* winApp_;

	//フェード状態
	FadeState state_ = FadeState::NONE;

	//フェードスプライト
	std::vector<FadeSprite> fadeSprites_;

	//カメラ
	MyEngine::Camera* camera_;

	//プレイヤー
	Player* player_;

	//プレイヤーの2D座標
	MyEngine::Vector2 playerPos2D_;

	//アルファ値
	float alpha_ = 0.0f;

	//タイマー
	float timer_ = 0.0f;

	//最大タイマー
	float maxTimer_ = 1.0f;

	//フェード速度
	float fadeSpeed_ = 0.02f;

	///=====================================================/// 
	/// ゲッター・セッター
	///=====================================================///
public:

	/// <summary>
	/// フェード状態の取得
	/// </summary>
	FadeState GetState() const { return state_; }

	/// <summary>
	/// フェード状態の設定
	/// </summary>
	void SetState(FadeState state) { state_ = state; }

	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	void SetPlayer(Player* ptr) { player_ = ptr; }

};
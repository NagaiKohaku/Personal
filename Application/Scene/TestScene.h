#pragma once

#include <Scene/BaseScene.h>

#include <Object/Base/GameObject.h>

#include <memory>

class TestScene : public MyEngine::BaseScene {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// タイトルシーンの各種オブジェクトを初期化する関数です
	/// </summary>
	void Initialize(EngineContext context) override;

	/// <summary>
	/// タイトルシーン終了時に各種オブジェクトを解放する関数です
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// タイトルシーンの毎フレーム更新処理を行う関数です
	/// </summary>
	void Update() override;

	/// <summary>
	/// タイトルシーンに必要な描画処理を行う関数です
	/// </summary>
	void Draw() override;

	/// <summary>
	/// デバッグ
	/// </summary>
	void ImGui() override;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	std::unique_ptr<MyEngine::GameObject> gameObject_;
};
#pragma once

#include <Scene/BaseScene.h>

#include "memory"

/// <summary>
/// シーンの状態を管理するクラスです。
/// </summary>

class SceneManager {

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
public:

	/// <summary>
	/// シーンの種類
	/// </summary>
	enum class SceneType {
		kTitle,
		kGame,
		kParticleEditor
	};

	///-------------------------------------------/// 
	///メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// SceneManagerのシングルトンインスタンスを取得します。
	/// </summary>
	static SceneManager* GetInstance();

	/// <summary>
	/// 現在設定されているシーンの初期化処理を実行します。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 現在設定されているシーンの更新処理を実行します。
	/// </summary>
	void Update();

	/// <summary>
	/// 現在設定されているシーンの描画処理を実行します。
	/// </summary>
	void Draw();

	/// <summary>
	/// 現在設定されているシーンのImGui処理を実行します。
	/// </summary>
	void ImGui();

	/// <summary>
	/// 現在のシーンを指定された種類の新しいシーンに切り替えます。
	/// </summary>
	void ChangeScene(SceneType sceneType);

	/// <summary>
	/// 指定された種類に応じたシーンオブジェクトを生成して返します。
	/// </summary>
	std::unique_ptr<MyEngine::BaseScene> CreateScene(SceneType sceneType);

	///-------------------------------------------/// 
	///メンバ変数
	///-------------------------------------------///
private:

	//現在のシーン
	std::unique_ptr<MyEngine::BaseScene> currentScene_;
};

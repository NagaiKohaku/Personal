#pragma once

#include "memory"

/// === 前方宣言 === ///
 
class BaseScene;

///=====================================================/// 
///シーンマネージャー
///=====================================================///
class SceneManager {

	///-------------------------------------------/// 
	/// メンバ構造体
	///-------------------------------------------///
public:

	/// <summary>
	/// シーンの種類
	/// </summary>
	enum SceneType {
		kTitle,
		kGame,
		kParticleEditor
	};

	///-------------------------------------------/// 
	///メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static SceneManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 現在のシーンの更新
	/// </summary>
	void Update();

	/// <summary>
	/// 現在のシーンの描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 現在のシーンのデバッグ
	/// </summary>
	void ImGui();

	/// <summary>
	/// シーンを変更する
	/// </summary>
	void ChangeScene(SceneType sceneType);

	/// <summary>
	/// シーンの生成
	/// </summary>
	/// <param name="sceneType"></param>
	/// <returns></returns>
	std::unique_ptr<BaseScene> CreateScene(SceneType sceneType);

	///-------------------------------------------/// 
	///メンバ変数
	///-------------------------------------------///
private:

	//現在のシーン
	std::unique_ptr<BaseScene> currentScene_;
};
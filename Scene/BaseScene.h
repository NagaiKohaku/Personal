#pragma once

/// === 前方宣言 === ///
class SceneManager;

///=====================================================/// 
/// ベースシーン
///=====================================================///
class BaseScene {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~BaseScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// シーンマネージャーのセット
	/// </summary>
	/// <param name="sceneManager">シーンマネージャー</param>
	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

	//シーンマネージャー
	SceneManager* sceneManager_ = nullptr;

private:

};
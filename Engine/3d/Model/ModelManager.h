#pragma once

#include "3d/Model/Model.h"

#include "memory"
#include "map"

/// === 前方宣言 === ///
class ModelCommon;

///=====================================================/// 
/// モデルマネージャ
///=====================================================///
class ModelManager {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static ModelManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// モデルの読み込み
	/// </summary>
	/// <param name="modelName">モデル名</param>
	/// <param name="modelFileName">モデルのファイル名</param>
	void LoadModel(const std::string& modelName, const std::string& modelFileName);

	/// <summary>
	/// モデルの検索
	/// </summary>
	/// <param name="modelName">モデル名</param>
	/// <returns>モデルデータ</returns>
	Model* FindModel(const std::string& modelName);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//モデル基底
	ModelCommon* modelCommon_;

	//モデルリスト
	std::map<std::string, std::unique_ptr<Model>> models_;
};
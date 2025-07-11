#pragma once

#include "3d/Model/Model.h"

#include "memory"
#include "map"

/// === 前方宣言 === ///
class ModelCommon;

///=====================================================/// 
/// モデルマネージャクラス
///=====================================================///
class ModelManager {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static ModelManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// モデルの読み込み
	/// </summary>
	/// <param name="modelName">モデル名</param>
	/// <param name="modelFileName">モデルのファイル名</param>
	void LoadModel(const std::string& modelName, const std::string& modelFileName);

	/// <summary>
	/// メッシュモデルの生成
	/// </summary>
	/// <param name="type"></param>
	/// <param name="modelName"></param>
	/// <param name="textureFilePath"></param>
	void CreateMeshModel(const std::string& modelName, MeshType type, const std::string& textureFilePath);

	/// <summary>
	/// モデルの検索
	/// </summary>
	/// <param name="modelName">モデル名</param>
	/// <returns>モデルデータ</returns>
	std::unique_ptr<Model> FindModel(const std::string& modelName);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//モデル基底
	ModelCommon* modelCommon_;

	//モデルリスト
	std::map<std::string, std::unique_ptr<Model>> models_;
};
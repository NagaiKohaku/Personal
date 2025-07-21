#pragma once

#include "LevelEditor/ObjectData.h"

#include "Math/Vector3.h"

#include "string"
#include "vector"
#include "map"

///=====================================================/// 
/// レベルデータローダークラス
///=====================================================///
class LevelDataLoader {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static LevelDataLoader* GetInstance();

	/// <summary>
	/// データの読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void Load(const std::string& filePath);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//レベルデータ
	std::map<std::string, std::vector<ObjectData>> levelData_;

	//ディレクトリパス
	const std::string directory_ = "Resource/Json/LevelData/";

	///-------------------------------------------///
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// レベルデータを取得
	/// </summary>
	/// <returns>レベルデータ</returns>
	const std::map<std::string, std::vector<ObjectData>>& GetLevelData() const {
		return levelData_;
	}

	/// <summary>
	/// オブジェクトの数を取得
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="type">タイプ</param>
	/// <returns>オブジェクトの数</returns>
	const int GetObjectCount(const std::string& fileName, const ObjectType type) const;

	/// <summary>
	/// 指定したタイプのオブジェクトデータを取得
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="type">タイプ</param>
	/// <returns>オブジェクトデータ</returns>
	const std::vector<ObjectData>& PickObjectData(const std::string& fileName, const ObjectType type) const;
};
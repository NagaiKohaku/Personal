#pragma once

#include "LevelEditor/ObjectData.h"

#include <Math/Vector/Vector3.h>

#include "string"
#include "vector"
#include "map"

/// <summary>
/// レベルデータを管理するクラスです。
/// </summary>
class LevelDataLoader {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// LevelDataLoaderのシングルトンインスタンスを取得します。
	/// </summary>
	static LevelDataLoader* GetInstance();

	/// <summary>
	/// レベルデータの読み込みを初期化します。
	/// </summary>
	void Initialize();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 指定されたJSONファイルからレベルデータを読み込みます。
	/// </summary>
	/// <param name="fileName">読み込むJSONファイルの相対パス</param>
	void Load(const std::string& filePath);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//レベルデータ
	std::map<std::string, std::vector<ObjectData>> levelData_;

	//ディレクトリパス
	std::string directory_;

	///-------------------------------------------///
	/// ゲッター・セッター
	///-------------------------------------------///
public:

	/// <summary>
	/// レベルデータを取得
	/// </summary>
	/// <returns>レベルデータ</returns>
	std::map<std::string, std::vector<ObjectData>>& GetLevelData(){
		return levelData_;
	}

	/// <summary>
	/// 指定したレベルデータ内で特定のタイプのオブジェクト数を取得します。
	/// </summary>
	/// <param name="fileName">対象のレベルデータのファイル名</param>
	/// <param name="type">カウントしたいオブジェクトのタイプ</param>
	/// <returns>指定したタイプのオブジェクトの数</returns>
	int GetObjectCount(const std::string& fileName, const ObjectType type);

	/// <summary>
	/// 指定したディレクトリ名に含まれるレベルデータの数を取得します。
	/// </summary>
	/// <param name="directoryName">対象のディレクトリ名（相対パスの先頭部分）</param>
	/// <returns>指定ディレクトリに属するレベルデータの件数</returns>
	int GetObjectDataCount(const std::string& directoryName);

	/// <summary>
	/// 指定したレベルデータから特定のタイプのオブジェクトデータを抽出します。
	/// </summary>
	/// <param name="fileName">対象のレベルデータのファイル名</param>
	/// <param name="type">抽出したいオブジェクトのタイプ</param>
	/// <returns>指定タイプのオブジェクトデータを格納したベクター</returns>
	std::vector<ObjectData> PickObjectData(const std::string& fileName, const ObjectType type);

};
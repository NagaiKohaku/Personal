#pragma once

#include "LevelEditor/ObjectData.h"

#include "Math/Vector3.h"

#include "string"
#include "vector"
#include "map"

/// <summary>
/// レベルデータを管理するクラスです。
/// </summary>
/// <remarks>
/// - JSON形式で保存されたレベルデータを読み込み、内部コンテナに保持します。
/// - 各オブジェクトの種類（PLAYER, ENEMY, OBJECTなど）やトランスフォーム情報を管理します。
/// - 指定ファイルやディレクトリ単位でオブジェクトの数を取得したり、特定タイプのオブジェクトデータを抽出することが可能です。
/// - シングルトンとして利用され、ゲーム全体で共通のレベルデータ管理を提供します。
/// </remarks>
class LevelDataLoader {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// LevelDataLoaderのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static LevelDataLoader* GetInstance();

	/// <summary>
	/// レベルデータの読み込みを初期化します。
	/// </summary>
	/// <remarks>
	/// - JSONファイルが格納されているディレクトリパスを設定します。
	/// - ディレクトリ内の全てのファイルを再帰的に走査します。
	/// - 拡張子が ".json" のファイルのみ対象とし、相対パスを取得します。
	/// - 取得した相対パスを使用して各レベルデータを読み込みます。
	/// </remarks>
	void Initialize();

	///-------------------------------------------/// 
	/// クラス内処理関数
	///-------------------------------------------///
private:

	/// <summary>
	/// 指定されたJSONファイルからレベルデータを読み込みます。
	/// </summary>
	/// <param name="fileName">読み込むJSONファイルの相対パス</param>
	/// <remarks>
	/// - ファイルを開き、JSON形式でデータを読み込みます。
	/// - レベルデータが正しい形式であるかチェックします（"name"キーが "scene" であることなど）。
	/// - "objects" 配列内の各オブジェクトを走査し、無効フラグが立っていればスキップします。
	/// - オブジェクトの "type" および "object_group" を確認し、ObjectType を設定します。
	/// - モデルのファイル名が存在する場合は ModelManager を用いてモデルを読み込みます。
	/// - トランスフォーム情報（平行移動、回転、スケーリング）を ObjectData に格納します。
	/// - 読み込んだオブジェクトデータをファイル名をキーとしてレベルデータコンテナに保存します。
	/// </remarks>
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
	/// <remarks>
	/// - ファイル名が存在しない場合は0を返します。
	/// - レベルデータ内の全オブジェクトを走査し、タイプが一致する場合にカウントを増加させます。
	/// </remarks>
	int GetObjectCount(const std::string& fileName, const ObjectType type);

	/// <summary>
	/// 指定したディレクトリ名に含まれるレベルデータの数を取得します。
	/// </summary>
	/// <param name="directoryName">対象のディレクトリ名（相対パスの先頭部分）</param>
	/// <returns>指定ディレクトリに属するレベルデータの件数</returns>
	/// <remarks>
	/// - レベルデータのキー（ファイル名）が指定されたディレクトリ名で始まる場合にカウントします。
	/// - ディレクトリ内の全てのレベルデータを走査し、該当するデータの数を返します。
	/// </remarks>
	int GetObjectDataCount(const std::string& directoryName);

	/// <summary>
	/// 指定したレベルデータから特定のタイプのオブジェクトデータを抽出します。
	/// </summary>
	/// <param name="fileName">対象のレベルデータのファイル名</param>
	/// <param name="type">抽出したいオブジェクトのタイプ</param>
	/// <returns>指定タイプのオブジェクトデータを格納したベクター</returns>
	/// <remarks>
	/// - ファイル名が存在しない場合は空のベクターを返します。
	/// - レベルデータ内の全オブジェクトを走査し、タイプが一致する場合にベクターに追加します。
	/// - 抽出結果として複数のオブジェクトデータを返すことが可能です。
	/// </remarks>
	std::vector<ObjectData> PickObjectData(const std::string& fileName, const ObjectType type);

};
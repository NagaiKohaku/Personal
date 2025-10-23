#pragma once

#include "3d/Model/ModelCommon.h"
#include "3d/Model/Model.h"

#include "memory"
#include "map"

/// <summary>
/// ゲーム内で使用するモデルを管理するクラスです。
/// </summary>
/// <remarks>
/// - ModelCommon を利用して DirectX 用の共通リソースにアクセスします。
/// - プリミティブメッシュを初期化時に生成します。
/// - OBJファイルやプリミティブメッシュからモデルを生成し、管理コンテナに登録します。
/// - 登録されたモデルは unique_ptr で保持され、モデル名をキーに検索可能です。
/// </remarks>
class ModelManager {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// ModelManagerのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static ModelManager* GetInstance();

	/// <summary>
	/// モデルマネージャを初期化し、基本的なプリミティブモデルを生成します。
	/// </summary>
	/// <remarks>
	/// - ModelCommon のインスタンスを取得します。
	/// - Plane, Ring, Cylinder, Sphere, Cube の各プリミティブモデルを生成します。
	/// - 各モデルには共通の白テクスチャを設定します。
	/// </remarks>
	void Initialize();

	/// <summary>
	/// 指定されたモデル名とファイル名からモデルを読み込み、管理コンテナに登録します。
	/// </summary>
	/// <param name="modelName">管理用のモデル名</param>
	/// <param name="modelFileName">モデルファイルの名前</param>
	/// <remarks>
	/// - すでに同名のモデルが読み込まれている場合は処理をスキップします。
	/// - ModelクラスのInitialize関数を使用して objファイルを読み込みます。
	/// - 読み込んだモデルはunique_ptrで管理され、マップコンテナに登録されます。
	/// </remarks>
	void LoadModel(const std::string& modelName, const std::string& modelFileName);

	/// <summary>
	/// 指定されたモデル名、ディレクトリ、ファイル名からモデルを読み込み、管理コンテナに登録します。
	/// </summary>
	/// <param name="modelName">管理用のモデル名</param>
	/// <param name="modelDirectory">モデルファイルが格納されているディレクトリ名</param>
	/// <param name="modelFileName">モデルファイルの名前</param>
	/// <remarks>
	/// - すでに同名のモデルが読み込まれている場合は処理をスキップします。
	/// - ModelクラスのInitialize関数を使用して objファイルを読み込みます。
	/// - 読み込んだモデルはunique_ptrで管理され、マップコンテナに登録されます。
	/// </remarks>
	void LoadModel(const std::string& modelName, const std::string& modelDirectory, const std::string& modelFileName);

	/// <summary>
	/// 指定したタイプのメッシュモデルを生成し、テクスチャを設定して管理コンテナに登録します。
	/// </summary>
	/// <param name="modelName">管理用のモデル名</param>
	/// <param name="type">生成するメッシュの種類</param>
	/// <param name="textureFilePath">モデルに適用するテクスチャファイルのパス</param>
	/// <remarks>
	/// - すでに同名のモデルが読み込まれている場合は処理をスキップします。
	/// - ModelクラスのInitialize関数を使用して objファイルを読み込みます。
	/// - 読み込んだモデルはunique_ptrで管理され、マップコンテナに登録されます。
	/// </remarks>
	void CreateMeshModel(const std::string& modelName, MeshType type, const std::string& textureFilePath);

	/// <summary>
	/// 登録済みモデルを検索し、見つかった場合は同じメッシュタイプで新しいモデルインスタンスを生成して返します。
	/// </summary>
	/// <param name="modelName">検索するモデルの名前</param>
	/// <returns>名前が合致したモデルをコピーしたインスタンス</returns>
	/// <remarks>
	/// - 返されるモデルは新規生成されるため、呼び出し元が独自に管理できます。
	/// </remarks>
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
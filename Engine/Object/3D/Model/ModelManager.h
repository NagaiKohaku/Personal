#pragma once

#include <Base\DirectXCommon.h>
#include <Object/Component/Geometry/Model/Model.h>

#include "memory"
#include "map"
#include <string>

namespace MyEngine {

	/// <summary>
	/// ゲーム内で使用するモデルを管理するクラスです。
	/// </summary>
	class ModelManager {

	private:

		struct ModelData {
			std::unique_ptr<Model> model;
			std::map<uint32_t, bool> instanceID;
			uint32_t instanceCount;
		};

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// ModelManagerのシングルトンインスタンスを取得します。
		/// </summary>
		static ModelManager* GetInstance();

		/// <summary>
		/// モデルマネージャを初期化し、基本的なプリミティブモデルを生成します。
		/// </summary>
		void Initialize(DirectXCommon* directCommonPtr);

		/// <summary>
		/// 指定されたモデル名とファイル名からモデルを読み込み、管理コンテナに登録します。
		/// </summary>
		/// <param name="modelName">管理用のモデル名</param>
		/// <param name="modelFileName">モデルファイルの名前</param>
		void LoadModel(const std::string& modelFileName);

		/// <summary>
		/// 指定したタイプのメッシュモデルを生成し、テクスチャを設定して管理コンテナに登録します。
		/// </summary>
		/// <param name="modelName">管理用のモデル名</param>
		/// <param name="type">生成するメッシュの種類</param>
		/// <param name="textureFilePath">モデルに適用するテクスチャファイルのパス</param>
		void CreateMeshModel(const std::string& modelName, MeshType type, const std::string& textureFilePath);

		/// <summary>
		/// 登録済みモデルを検索し、見つかった場合は同じメッシュタイプで新しいモデルインスタンスを生成して返します。
		/// </summary>
		/// <param name="modelName">検索するモデルの名前</param>
		/// <returns>名前が合致したモデルをコピーしたインスタンス</returns>
		Model* GetModel(const std::string& modelName) const;

		bool FindModel(const std::string& modelName) const;

		uint32_t GetFreeInstanceID(const std::string& modelName) const;

		uint32_t GetInstanceCount(const std::string& modelName) const;

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//モデル基底
		DirectXCommon* directCommon_;

		//モデルリスト
		std::map<std::string, ModelData> models_;

		const uint32_t kInstanceMax = 100;
	};
}
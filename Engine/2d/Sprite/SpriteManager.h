#pragma once
#include "Sprite.h"

#include "memory"
#include "map"

namespace MyEngine {

	/// <summary>
	/// スプライトの管理を行うクラス。
	/// </summary>
	class SpriteManager {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// SpriteManagerのシングルトンインスタンスを取得します。
		/// </summary>
		static SpriteManager* GetInstance();

		/// <summary>
		/// SpriteManagerを初期化します。
		/// </summary>
		void Initialize(SpriteCommon* spriteCommonPtr);

		/// <summary>
		/// 指定した名前のスプライトを検索し、未登録であれば読み込みます。
		/// </summary>
		/// <param name="spriteName"> スプライト名 </param>
		/// <param name="spriteFileName"> スプライトファイル名 </param>
		void LoadSprite(const std::string& spriteName);

		/// <summary>
		/// 指定した名前のスプライトを検索して新しいインスタンスを生成します。
		/// </summary>
		/// <param name="spriteName">検索するスプライトの名前</param>
		/// <returns>
		/// 新しいスプライトインスタンス (登録されていなかったらnullptr)
		/// </returns>
		std::unique_ptr<Sprite> FindSprite(const std::string& spriteName);

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//スプライト基底
		SpriteCommon* spriteCommon_;

		//スプライトリスト
		std::map<std::string, std::unique_ptr<Sprite>> sprites_;

	public:

		std::map<std::string, Sprite*> GetSpriteList();

		std::vector<std::string> GetSpriteNameList();
	};
}
#pragma once
#include "Sprite.h"

#include "memory"
#include "map"

///=====================================================/// 
/// スプライトマネージャー
///=====================================================///
class SpriteManager {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns>インスタンス</returns>
	static SpriteManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// スプライトの読み込み
	/// </summary>
	void LoadSprite(const std::string& spriteName, const std::string& spriteFileName);

	/// <summary>
	/// スプライトの検索
	/// </summary>
	/// <param name="spriteName">スプライト名</param>
	/// <returns>スプライトデータ</returns>
	Sprite* FindSprite(const std::string& spriteName);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//スプライト基底
	SpriteCommon* spriteCommon_;

	//スプライトリスト
	std::map<std::string, std::unique_ptr<Sprite>> sprites_;
};
#pragma once
#include "Sprite.h"

#include "memory"
#include "map"

/// <summary>
/// スプライトの管理を行うクラス。
/// </summary>
/// <remarks>
/// - 読み込まれたスプライトを一元管理
/// - 登録済みのスプライトを基に別ポインタのスプライトインスタンスを生成・引き渡す
/// </remarks>
class SpriteManager {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// SpriteManagerのシングルトンインスタンスを取得します。
	/// </summary>
	/// <remarks>
	/// 返り値に静的インスタンスを返します。
	/// </remarks>
	static SpriteManager* GetInstance();

	/// <summary>
	/// SpriteManagerを初期化します。
	/// </summary>
	/// <remarks>
	/// SpriteCommonのシングルトンインスタンスを取得し、内部メンバ(spriteCommon_)に保持します。
	/// </remarks>
	void Initialize();

	/// <summary>
	/// 指定した名前のスプライトを検索し、未登録であれば読み込みます。
	/// </summary>
	/// <param name="spriteName"> スプライト名 </param>
	/// <param name="spriteFileName"> スプライトファイル名 </param>
	/// <remarks>
	/// - リストから検索して登録されているかを確認
	///   - 存在する場合は何もせず早期リターン
	/// - スプライトを生成
	/// - 指定されたファイルからスプライトを初期化
	/// - スプライト名と生成したスプライトをリストに登録
	/// </remarks>
	void LoadSprite(const std::string& spriteName, const std::string& spriteFileName);

	/// <summary>
	/// 指定した名前のスプライトを検索して新しいインスタンスを生成します。
	/// </summary>
	/// <param name="spriteName">検索するスプライトの名前</param>
	/// <returns>
	/// 新しいスプライトインスタンス (登録されていなかったらnullptr)
	/// </returns>
	/// <remarks>
	/// - リストから検索して登録されているかを確認
	///   - 存在する場合は新しくスプライトを生成後、インスタンスを返す
	/// - 登録されていなかったらnullptrを返す
	/// </remarks>
	std::unique_ptr<Sprite> FindSprite(const std::string& spriteName);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	//スプライト基底
	SpriteCommon* spriteCommon_;

	//スプライトリスト
	std::map<std::string, std::unique_ptr<Sprite>> sprites_;
};
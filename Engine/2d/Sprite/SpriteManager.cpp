#include "SpriteManager.h"

#include "2d/Sprite/SpriteCommon.h"

#include "fstream"
#include "filesystem"

namespace MyEngine {

	///=====================================================/// 
	/// SpriteManagerのシングルトンインスタンスを取得
	///=====================================================///
	SpriteManager* SpriteManager::GetInstance() {
		static SpriteManager instance;
		return &instance;
	}

	///=====================================================/// 
	/// SpriteManagerを初期化
	///=====================================================///
	void SpriteManager::Initialize() {

		//スプライト基底のインスタンスを取得
		spriteCommon_ = SpriteCommon::GetInstance();

		//スプライトデータを読み込む
		for (const auto& entry : std::filesystem::directory_iterator("Resource/Sprite")) {

			if (entry.is_directory()) {

				LoadSprite(entry.path().filename().string());
			}
		}
	}

	///=====================================================/// 
	/// 指定した名前のスプライトを検索し、未登録であれば読み込む
	///=====================================================///
	void SpriteManager::LoadSprite(const std::string& spriteName) {

		//引数の名前のスプライトが登録されているかを確認
		if (sprites_.contains(spriteName)) {

			//登録済みであれば早期リターン
			return;
		}

		//登録するためのスプライトを宣言
		std::unique_ptr<Sprite> newSprite = std::make_unique<Sprite>();

		//スプライトを読み込む
		newSprite->Initialize(spriteName);

		//リストに登録
		sprites_.insert(std::make_pair(spriteName, std::move(newSprite)));
	}

	///=====================================================///
	/// 指定した名前のスプライトを検索して新しいインスタンスを生成
	///=====================================================///
	std::unique_ptr<Sprite> SpriteManager::FindSprite(const std::string& spriteName) {

		//引数の名前のスプライトが登録されているかを確認
		if (sprites_.contains(spriteName)) {

			//登録済みのスプライトからテクスチャパスを取得
			std::string texturePath = sprites_.at(spriteName)->GetFileName();

			//スプライトを生成
			std::unique_ptr<Sprite> newSprite = std::make_unique<Sprite>();

			//初期化
			newSprite->Initialize(texturePath);

			//新しく生成したスプライトを返す
			return std::move(newSprite);
		}

		//登録されていなかったのでfalseを返す
		return nullptr;
	}

	std::map<std::string, Sprite*> SpriteManager::GetSpriteList() {

		std::map<std::string, Sprite*> list;

		for (auto& [name, sprite] : sprites_) {

			list.insert(std::make_pair(name, sprite.get()));
		}

		return list;
	}

	std::vector<std::string> SpriteManager::GetSpriteNameList() {

		std::vector<std::string> list;

		for (auto& [name, sprite] : sprites_) {

			list.push_back(name);
		}

		return list;
	}
}
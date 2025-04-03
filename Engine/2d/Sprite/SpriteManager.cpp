#include "SpriteManager.h"

#include "2d/Sprite/SpriteCommon.h"

///=====================================================/// 
/// 静的インスタンスの生成
///=====================================================///
SpriteManager* SpriteManager::GetInstance() {
	static SpriteManager instance;
	return &instance;
}

///=====================================================/// 
/// 初期化
///=====================================================///
void SpriteManager::Initialize() {

	//スプライト基底のインスタンスを取得
	spriteCommon_ = SpriteCommon::GetInstance();
}

///=====================================================/// 
/// スプライトの読み込み
///=====================================================///
void SpriteManager::LoadSprite(const std::string& spriteName,const std::string& spriteFileName) {

	//引数の名前のスプライトが登録されているかを確認
	if (sprites_.contains(spriteName)) {

		//登録済みであれば早期リターン
		return;
	}

	//登録するためのスプライトを宣言
	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();

	//スプライトを読み込む
	sprite->Initialize("Resource/Sprite/" + spriteFileName + ".png");

	//リストに登録
	sprites_.insert(std::make_pair(spriteName, std::move(sprite)));
}

///=====================================================///
/// スプライトの検索
///=====================================================///
Sprite* SpriteManager::FindSprite(const std::string& spriteName) {

	//引数の名前のスプライトが登録されているかを確認
	if (sprites_.contains(spriteName)) {

		//登録されていたスプライトを返す
		return sprites_.at(spriteName).get();
	}

	//登録されていなかったのでfalseを返す
	return nullptr;
}

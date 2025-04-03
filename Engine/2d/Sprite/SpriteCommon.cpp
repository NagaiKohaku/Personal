#include "SpriteCommon.h"

#include "Base/DirectXCommon.h"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
SpriteCommon* SpriteCommon::GetInstance() {
	static SpriteCommon instance;
	return &instance;
}

///=====================================================/// 
/// 初期化処理
///=====================================================///
void SpriteCommon::Initialize() {

	//DirectX基底のインスタンスを取得
	dxCommon_ = DirectXCommon::GetInstance();
}

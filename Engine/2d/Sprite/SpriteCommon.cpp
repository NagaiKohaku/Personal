#include "SpriteCommon.h"

#include "Base/DirectXCommon.h"

///=====================================================/// 
/// シングルトンインスタンスを取得
///=====================================================///
SpriteCommon* SpriteCommon::GetInstance() {
	static SpriteCommon instance;
	return &instance;
}

///=====================================================/// 
/// 初期化
///=====================================================///
void SpriteCommon::Initialize() {

	//DirectX基底のインスタンスを取得
	dxCommon_ = DirectXCommon::GetInstance();
}

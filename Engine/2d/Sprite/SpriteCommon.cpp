#include "SpriteCommon.h"

namespace MyEngine {

	///=====================================================/// 
	/// SpriteCommonを初期化
	///=====================================================///
	void SpriteCommon::Initialize(DirectXCommon* dxCommonPtr) {

		//DirectX基底のインスタンスを取得
		dxCommon_ = dxCommonPtr;
	}
}
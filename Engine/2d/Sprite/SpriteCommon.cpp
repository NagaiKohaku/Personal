#include "SpriteCommon.h"

namespace MyEngine {

	///=====================================================/// 
	/// SpriteCommonのシングルトンインスタンスを取得
	///=====================================================///
	SpriteCommon* SpriteCommon::GetInstance() {
		static SpriteCommon instance;
		return &instance;
	}

	///=====================================================/// 
	/// SpriteCommonを初期化
	///=====================================================///
	void SpriteCommon::Initialize(DirectXCommon* dxCommonPtr) {

		//DirectX基底のインスタンスを取得
		dxCommon_ = dxCommonPtr;
	}
}
#include "ModelCommon.h"

namespace MyEngine {

	///=====================================================/// 
	/// ModelCommonの初期化
	///=====================================================///
	void ModelCommon::Initialize(DirectXCommon* dxCommonPtr) {

		//DirectX基底のインスタンスを取得
		dxCommon_ = dxCommonPtr;
	}
}
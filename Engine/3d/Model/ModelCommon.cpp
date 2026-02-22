#include "ModelCommon.h"

namespace MyEngine {

	///=====================================================/// 
	/// ModelCommonのシングルトンインスタンスを取得
	///=====================================================///
	ModelCommon* ModelCommon::GetInstance() {
		static ModelCommon instance;
		return &instance;
	}

	///=====================================================/// 
	/// ModelCommonの初期化
	///=====================================================///
	void ModelCommon::Initialize(DirectXCommon* dxCommonPtr) {

		//DirectX基底のインスタンスを取得
		dxCommon_ = dxCommonPtr;
	}
}
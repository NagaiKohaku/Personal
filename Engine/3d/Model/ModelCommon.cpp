#include "ModelCommon.h"

namespace MyEngine {

	ModelCommon::ModelCommon() = default;

	ModelCommon::~ModelCommon() = default;

	///=====================================================/// 
	/// ModelCommonの初期化
	///=====================================================///
	void ModelCommon::Initialize(DirectXCommon* dxCommonPtr) {

		//DirectX基底のインスタンスを取得
		dxCommon_ = dxCommonPtr;
	}
}
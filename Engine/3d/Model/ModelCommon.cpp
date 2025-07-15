#include "ModelCommon.h"

#include "Base/DirectXCommon.h"

///=====================================================/// 
/// シングルトンインスタンスを取得
///=====================================================///
ModelCommon* ModelCommon::GetInstance() {
	static ModelCommon instance;
	return &instance;
}

///=====================================================/// 
/// 初期化
///=====================================================///
void ModelCommon::Initialize() {

	//DirectX基底のインスタンスを取得
	dxCommon_ = DirectXCommon::GetInstance();
}
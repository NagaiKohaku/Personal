#include "ModelCommon.h"

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
void ModelCommon::Initialize() {

	//DirectX基底のインスタンスを取得
	dxCommon_ = DirectXCommon::GetInstance();
}
#include "ModelCommon.h"

#include "Base/DirectXCommon.h"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
ModelCommon* ModelCommon::GetInstance() {
	static ModelCommon instance;
	return &instance;
}

///=====================================================/// 
/// 初期化処理
///=====================================================///
void ModelCommon::Initialize() {

	//DirectX基底のインスタンスを取得
	dxCommon_ = DirectXCommon::GetInstance();
}
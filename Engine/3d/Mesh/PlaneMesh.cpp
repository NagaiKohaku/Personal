#include "PlaneMesh.h"

#include "Base/DirectXCommon.h"

///=====================================================/// 
/// PlaneMeshの初期化
///=====================================================///
void PlaneMesh::Initialize() {

	/// === 頂点データの設定 === ///

	//左下
	vertexData_.push_back({
		{ 0.0f - anchorPoint_.x,1.0f - anchorPoint_.y,0.0f - anchorPoint_.z,1.0f },
		{ 0.0f,1.0f },
		{ 0.0f,0.0f,-1.0f }
		});

	//左上
	vertexData_.push_back({
		{ 0.0f - anchorPoint_.x,0.0f - anchorPoint_.y,0.0f - anchorPoint_.z,1.0f },
		{ 0.0f,0.0f },
		{ 0.0f,0.0f,-1.0f }
		});

	//右下
	vertexData_.push_back({
		{ 1.0f - anchorPoint_.x,1.0f - anchorPoint_.y,0.0f - anchorPoint_.z,1.0f },
		{ 1.0f,1.0f },
		{ 0.0f,0.0f,-1.0f }
		});

	//右上
	vertexData_.push_back({
		{ 1.0f - anchorPoint_.x,0.0f - anchorPoint_.y,0.0f - anchorPoint_.z,1.0f },
		{ 1.0f,0.0f },
		{ 0.0f,0.0f,-1.0f }
		});

	/// === 頂点インデックスデータの設定 === ///

	//左下
	indexData_.push_back(0);

	//左上
	indexData_.push_back(1);

	//右下
	indexData_.push_back(2);

	//左上
	indexData_.push_back(1);

	//右上
	indexData_.push_back(3);

	//右下
	indexData_.push_back(2);

	//リソースの初期化
	MeshBase::Initialize();
}
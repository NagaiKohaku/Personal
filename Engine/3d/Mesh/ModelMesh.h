#pragma once

#include "3d/Mesh/MeshBase.h"

/// <summary>
/// モデルメッシュ（Model Mesh）を管理するクラスです。
/// </summary>
/// <remarks>
/// - MeshBase を継承しており、基本的なメッシュ機能を利用可能です。
/// - 初期化前に設定された頂点データとインデックスデータを基に生成されます。
/// </remarks>
class ModelMesh : public MeshBase {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// <summary>
	/// ModelMeshの初期化を行います。
	/// </summary>
	/// <remarks>
	/// - 頂点リソースとインデックスリソースを生成します。。
	/// - vertexCount_ と indexCount_ は事前に設定されている必要があります。
	/// </remarks>
	void Initialize() override;
};
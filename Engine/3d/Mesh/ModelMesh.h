#pragma once

#include "3d/Mesh/MeshBase.h"

namespace MyEngine {

	/// <summary>
	/// モデルメッシュ（Model Mesh）を管理するクラスです。
	/// </summary>
	class ModelMesh : public MeshBase {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// ModelMeshの初期化を行います。
		/// </summary>
		void Initialize() override;
	};
}
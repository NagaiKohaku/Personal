#pragma once

#include "3d/Mesh/MeshBase.h"

#include <Math/Vector/Vector3.h>

namespace MyEngine {

	/// <summary>
	/// 立方体メッシュ（Cube Mesh）を管理するクラスです。
	/// </summary>
	class CubeMesh : public MeshBase {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// CubeMeshの初期化を行います。
		/// </summary>
		void Initialize() override;

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//サイズ
		Vector3 size_;
	};
}
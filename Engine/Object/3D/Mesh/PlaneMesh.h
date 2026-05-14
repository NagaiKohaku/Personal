#pragma once

#include <Object/Component/Geometry/Mesh/MeshBase.h>

namespace MyEngine {

	/// <summary>
	/// 平面メッシュ（Plane Mesh）を管理するクラスです。
	/// </summary>
	class PlaneMesh : public MeshBase {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// PlaneMeshの初期化を行います。
		/// </summary>
		void Initialize(DirectXCommon* dxCommonPtr) override;

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//アンカーポイント
		Vector3 anchorPoint_ = { 0.5f,0.5f,0.0f };

	};
}
#pragma once

#include <Object/Component/Geometry/Mesh/MeshBase.h>

namespace MyEngine {

	/// <summary>
	/// 球面体メッシュ（Sphere Mesh）を管理するクラスです。
	/// </summary>
	class SphereMesh : public MeshBase {

		///-------------------------------------------/// 
		/// メンバ関数
		///-------------------------------------------///
	public:

		/// <summary>
		/// SphereMeshの初期化を行います。
		/// </summary>
		void Initialize(DirectXCommon* dxCommonPtr) override;

		///-------------------------------------------/// 
		/// メンバ変数
		///-------------------------------------------///
	private:

		//緯度の分割数
		const uint32_t kLatitudeCount = 16;

		//経度の分割数
		const uint32_t kLongitudeCount = 16;

		//中心点
		Vector3 center_ = { 0.0f, 0.0f, 0.0f };

		//半径
		float radius_ = 1.0f;
	};
}
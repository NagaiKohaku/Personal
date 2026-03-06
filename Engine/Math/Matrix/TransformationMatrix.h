#pragma once

#include <Math/Matrix/Matrix4x4.h>

namespace MyEngine {

	//座標変換行列データ
	struct TransformationMatrix final {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverseTranspose;
	};
}
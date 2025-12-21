#pragma once

#include <Math/Vector3.h>

//アニメーションのキーフレーム
struct AnimPoint {
	Vector3 playerPos;
	Vector3 cameraRot;
	float time;
	float mag;
};
#include "Easing.h"

#define _USE_MATH_DEFINES

#include "cmath"

///=====================================================///
///Lerp関数
///=====================================================///
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;

	result.x = (1.0f - t) * v1.x + t * v2.x;
	result.y = (1.0f - t) * v1.y + t * v2.y;
	result.z = (1.0f - t) * v1.z + t * v2.z;

	return result;
}

///=====================================================///
///SLerp関数
///=====================================================///
Vector3 SLerp(const Vector3& v1, const Vector3& v2, float t) {
	// なす角の計算
	float angle = std::cosf(Dot(v1, v2));

	// 線形補間を計算する
	float scaleV1 = std::sinf((1.0f - t) * angle) / std::sinf(angle);

	float scaleV2 = std::sinf(t * angle) / std::sinf(angle);

	Vector3 result;

	result.x = scaleV1 * v1.x + scaleV2 * v2.x;
	result.y = scaleV1 * v1.y + scaleV2 * v2.y;
	result.z = scaleV1 * v1.z + scaleV2 * v2.z;

	return result;
}

float Lerp(float n1, float n2, float t) {

	float result;

	result = (1.0f - t) * n1 + t * n2;

	return result;
}

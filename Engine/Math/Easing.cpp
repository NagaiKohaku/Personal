#include "Easing.h"

#define _USE_MATH_DEFINES

#include "cmath"

///=====================================================///
///Lerp関数
///=====================================================///
float Lerp(float n1, float n2, float t) {

	float result;

	result = (1.0f - t) * n1 + t * n2;

	return result;
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;

	result.x = (1.0f - t) * v1.x + t * v2.x;
	result.y = (1.0f - t) * v1.y + t * v2.y;
	result.z = (1.0f - t) * v1.z + t * v2.z;

	return result;
}

Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t) {
	Vector4 result;

	result.x = (1.0f - t) * v1.x + t * v2.x;
	result.y = (1.0f - t) * v1.y + t * v2.y;
	result.z = (1.0f - t) * v1.z + t * v2.z;
	result.w = (1.0f - t) * v1.w + t * v2.w;

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

///=====================================================/// 
///EaseIn関数
///=====================================================///
Vector3 EaseIn(const Vector3& v1, const Vector3& v2, float t, float mag) {

	float easeT = powf(t, mag);

	Vector3 result;

	result = Lerp(v1, v2, easeT);

	return result;
}

Vector4 EaseIn(const Vector4& v1, const Vector4& v2, float t, float mag) {

	float easeT = powf(t, mag);

	Vector4 result;

	result = Lerp(v1, v2, easeT);

	return result;
}

///=====================================================/// 
///EaseOut関数
///=====================================================///
Vector3 EaseOut(const Vector3& v1, const Vector3& v2, float t, float mag) {

	float easeT = 1.0f - powf(1.0f - t,mag);

	Vector3 result;

	result = Lerp(v1, v2, easeT);

	return result;
}

Vector4 EaseOut(const Vector4& v1, const Vector4& v2, float t, float mag) {

	float easeT = 1.0f - powf(1.0f - t, mag);

	Vector4 result;

	result = Lerp(v1, v2, easeT);

	return result;
}

Vector3 EaseOutCirc(const Vector3& v1, const Vector3& v2, float t, float mag) {

	float easeT = sqrtf(1.0f - powf(1.0f - t, mag));

	Vector3 result;

	result = Lerp(v1, v2, easeT);

	return result;
}

Vector3 EaseOutBack(const Vector3& v1, const Vector3& v2, float t, float mag) {

	float c1 = mag;

	float c3 = c1 + 1;

	float easeT = 1.0f + c3 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f);

	Vector3 result;

	result = Lerp(v1, v2, easeT);

	return result;
}

///=====================================================/// 
///EaseInOut関数
///=====================================================///
Vector3 EaseInOut(const Vector3& v1, const Vector3& v2, float t, float mag) {

	float easeT = t < 0.5f ? powf(2.0f,mag - 1.0f) * powf(t,mag) : 1.0f - powf(-2.0f, mag) / 2.0f;

	Vector3 result;

	result = Lerp(v1, v2, easeT);

	return result;
}

Vector4 EaseInOut(const Vector4& v1, const Vector4& v2, float t, float mag) {

	float easeT = t < 0.5f ? powf(2.0f, mag - 1.0f) * powf(t, mag) : 1.0f - powf(-2.0f, mag) / 2.0f;

	Vector4 result;

	result = Lerp(v1, v2, easeT);

	return result;
}

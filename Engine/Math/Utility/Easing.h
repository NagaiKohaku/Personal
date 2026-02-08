#pragma once

#include <Math/Vector/Vector2.h>
#include <Math/Vector/Vector3.h>
#include <Math/Vector/Vector4.h>

namespace MyEngine {

	enum class EaseType {
		LINEAR,
		EASE_OUT,
		EASE_IN,
		EASE_INOUT
	};

	template<typename T>
	T Ease(const T& from, const T& to, float t, EaseType type, float mag);

	// 明示的インスタンス宣言
	extern template float   Ease<float>(const float&, const float&, float, EaseType, float mag);

	extern template Vector2 Ease<Vector2>(const Vector2&, const Vector2&, float, EaseType, float mag);

	extern template Vector3 Ease<Vector3>(const Vector3&, const Vector3&, float, EaseType, float mag);

	extern template Vector4 Ease<Vector4>(const Vector4&, const Vector4&, float, EaseType, float mag);

	/// <summary>
	/// ラープ関数 : float
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	float Lerp(float n1, float n2, float t);

	/// <summary>
	/// ラープ関数 : Vector2
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t);

	/// <summary>
	/// ラープ関数 : Vector3
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	/// <summary>
	/// ラープ関数 : Vector4
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t);

	/// <summary>
	/// Sラープ関数
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector3 SLerp(const Vector3& v1, const Vector3& v2, float t);

	/// <summary>
	/// EaseIn関数
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <param name="mag"></param>
	/// <returns></returns>
	Vector2 EaseIn(const Vector2& v1, const Vector2& v2, float t, float mag = 2.0f);

	/// <summary>
	/// EaseIn関数
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <param name="mag"></param>
	/// <returns></returns>
	Vector3 EaseIn(const Vector3& v1, const Vector3& v2, float t, float mag = 2.0f);

	/// <summary>
	/// EaseIn関数
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <param name="mag"></param>
	/// <returns></returns>
	Vector4 EaseIn(const Vector4& v1, const Vector4& v2, float t, float mag = 2.0f);

	/// <summary>
	/// EaseOut関数
	/// </summary>
	/// <param name="n1"></param>
	/// <param name="n2"></param>
	/// <param name="t"></param>
	/// <param name="mag"></param>
	/// <returns></returns>
	float EaseOut(const float& n1, const float& n2, float t, float mag = 2.0f);

	/// <summary>
	/// EaseOut関数
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <param name="mag"></param>
	/// <returns></returns>
	Vector2 EaseOut(const Vector2& v1, const Vector2& v2, float t, float mag = 2.0f);

	/// <summary>
	/// EaseOut関数
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <param name="mag"></param>
	/// <returns></returns>
	Vector3 EaseOut(const Vector3& v1, const Vector3& v2, float t, float mag = 2.0f);

	/// <summary>
	/// EaseOut関数
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <param name="mag"></param>
	/// <returns></returns>
	Vector4 EaseOut(const Vector4& v1, const Vector4& v2, float t, float mag = 2.0f);

	/// <summary>
	/// EaseOutCirc関数
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <param name="mag"></param>
	/// <returns></returns>
	Vector3 EaseOutCirc(const Vector3& v1, const Vector3& v2, float t, float mag = 2.0f);

	/// <summary>
	/// EaseOutBack関数
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <param name="mag"></param>
	/// <returns></returns>
	Vector3 EaseOutBack(const Vector3& v1, const Vector3& v2, float t, float mag = 2.0f);

	/// <summary>
	/// EaseInOut関数
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <param name="mag"></param>
	/// <returns></returns>
	Vector3 EaseInOut(const Vector3& v1, const Vector3& v2, float t, float mag = 2.0f);

	/// <summary>
	/// EaseInOut関数
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="t"></param>
	/// <param name="mag"></param>
	/// <returns></returns>
	Vector4 EaseInOut(const Vector4& v1, const Vector4& v2, float t, float mag = 2.0f);
}
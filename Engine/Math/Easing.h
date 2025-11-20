#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

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
Vector2 EaseIn(const Vector2& v1, const Vector2& v2, float t, float mag);

/// <summary>
/// EaseIn関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <param name="mag"></param>
/// <returns></returns>
Vector3 EaseIn(const Vector3& v1, const Vector3& v2, float t, float mag);

/// <summary>
/// EaseIn関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <param name="mag"></param>
/// <returns></returns>
Vector4 EaseIn(const Vector4& v1, const Vector4& v2, float t, float mag);

/// <summary>
/// EaseOut関数
/// </summary>
/// <param name="n1"></param>
/// <param name="n2"></param>
/// <param name="t"></param>
/// <param name="mag"></param>
/// <returns></returns>
float EaseOut(const float& n1, const float& n2, float t, float mag);

/// <summary>
/// EaseOut関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <param name="mag"></param>
/// <returns></returns>
Vector2 EaseOut(const Vector2& v1, const Vector2& v2, float t, float mag);

/// <summary>
/// EaseOut関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <param name="mag"></param>
/// <returns></returns>
Vector3 EaseOut(const Vector3& v1, const Vector3& v2, float t, float mag);

/// <summary>
/// EaseOut関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <param name="mag"></param>
/// <returns></returns>
Vector4 EaseOut(const Vector4& v1, const Vector4& v2, float t, float mag);

/// <summary>
/// EaseOutCirc関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <param name="mag"></param>
/// <returns></returns>
Vector3 EaseOutCirc(const Vector3& v1, const Vector3& v2, float t, float mag);

/// <summary>
/// EaseOutBack関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <param name="mag"></param>
/// <returns></returns>
Vector3 EaseOutBack(const Vector3& v1, const Vector3& v2, float t, float mag);

/// <summary>
/// EaseInOut関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <param name="mag"></param>
/// <returns></returns>
Vector3 EaseInOut(const Vector3& v1, const Vector3& v2, float t, float mag);

/// <summary>
/// EaseInOut関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <param name="mag"></param>
/// <returns></returns>
Vector4 EaseInOut(const Vector4& v1, const Vector4& v2, float t, float mag);
#pragma once
#include "Vector3.h"

/// <summary>
/// ラープ関数 : Vector3
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

/// <summary>
/// Sラープ関数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 SLerp(const Vector3& v1, const Vector3& v2, float t);

/// <summary>
/// ラープ関数 : float
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
float Lerp(float n1, float n2, float t);
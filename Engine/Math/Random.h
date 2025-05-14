#pragma once

#include "random"

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

float RandomFloat(float min, float max);

Vector2 RandomVector2(Vector2 min, Vector2 max);

Vector3 RandomVector3(Vector3 min, Vector3 max);

Vector4 RandomVector4(Vector4 min, Vector4 max);

Vector2 RandomRangeVector2(Vector2 num, Vector2 range);

Vector3 RandomRangeVector3(Vector3 num, Vector3 range);

Vector4 RandomRangeVector4(Vector4 num, Vector4 range);
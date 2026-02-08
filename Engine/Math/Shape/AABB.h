#pragma once

#include <Math/Vector/Vector3.h>

namespace MyEngine {

	struct AABB {
		Vector3 min; //最小値
		Vector3 max; //最大値
	};
}
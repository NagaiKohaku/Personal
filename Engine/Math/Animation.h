#include "Math/NodeAnimation.h"

#include "map"
#include "string"

struct Animation {
	float duration;
	std::map<std::string, NodeAnimation> nodeAnimations;
};
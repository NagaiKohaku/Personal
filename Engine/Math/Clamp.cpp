#include "Clamp.h"

float ClampMinZero(float num) {

	if (num <= 0.0f) {
		num = 0.0f;
	}

	return num;
}
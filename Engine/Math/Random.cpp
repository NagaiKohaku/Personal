#include "Random.h"

float RandomFloat(float min, float max) {
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> dist(min, max);
	return dist(randomEngine);
}

Vector2 RandomVector2(Vector2 min, Vector2 max) {

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	std::uniform_real_distribution<float> distX(min.x, max.x);
	std::uniform_real_distribution<float> distY(min.y, max.y);

	return Vector2(distX(randomEngine), distY(randomEngine));
}

Vector3 RandomVector3(Vector3 min, Vector3 max) {

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	std::uniform_real_distribution<float> distX(min.x, max.x);
	std::uniform_real_distribution<float> distY(min.y, max.y);
	std::uniform_real_distribution<float> distZ(min.z, max.z);

	return Vector3(distX(randomEngine), distY(randomEngine), distZ(randomEngine));
}

Vector4 RandomVector4(Vector4 min, Vector4 max) {

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	std::uniform_real_distribution<float> distX(min.x, max.x);
	std::uniform_real_distribution<float> distY(min.y, max.y);
	std::uniform_real_distribution<float> distZ(min.z, max.z);
	std::uniform_real_distribution<float> distW(min.w, max.w);

	return Vector4(distX(randomEngine), distY(randomEngine), distZ(randomEngine), distW(randomEngine));
}
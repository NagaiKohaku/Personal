#pragma once

#include "Math/Vector3.h"

#include "string"
#include "vector"

class LevelDataLoader {

public:

	struct Object {
		std::string filename;
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;
	};

public:

	static LevelDataLoader* GetInstance();

	void Load(const std::string& filePath);

public:

	const std::vector<Object>& GetObjects() const {
		return objects_;
	}

private:

	std::vector<Object> objects_;

	const std::string directory_ = "Resource/Json/LevelData/";
};
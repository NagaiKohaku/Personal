#pragma once

#include "Math/Vector3.h"

#include "string"
#include "vector"
#include "map"

class LevelDataLoader {

public:

	enum ObjectType {
		NONE,
		PLAYER,
		ENEMY,
		OBJECT
	};

	struct ObjectData {
		std::string filename;
		ObjectType type;
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;
	};

public:

	static LevelDataLoader* GetInstance();

	void Load(const std::string& filePath);

private:

	std::map<std::string, std::vector<ObjectData>> objects_;

	const std::string directory_ = "Resource/Json/LevelData/";

public:

	const std::map<std::string, std::vector<ObjectData>>& GetObjects() const {
		return objects_;
	}

	const int GetObjectCount(const std::string& fileName, const ObjectType type) const;

	const std::vector<ObjectData>& PickObjectData(const std::string& fileName, const ObjectType type) const;

};
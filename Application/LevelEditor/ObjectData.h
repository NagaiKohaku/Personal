#pragma once

#include "Math/Vector3.h"

#include "string"

//オブジェクトのタイプ
enum class ObjectType {
	NONE,
	PLAYER,
	ENEMY,
	OBJECT
};

//オブジェクトのデータ構造体
struct ObjectData {
	std::string filename;
	ObjectType type;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};

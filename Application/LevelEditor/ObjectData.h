#pragma once

#include <Math/Vector/Vector3.h>

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
	MyEngine::Vector3 position;
	MyEngine::Vector3 rotation;
	MyEngine::Vector3 scale;
};
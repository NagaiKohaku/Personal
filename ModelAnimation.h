#pragma once

#include "Math/Animation.h"

#include "memory"

class Model;

class ModelAnimation {

public:

	Animation LoadAnimationFile(const std::string& filePath);

private:

	std::string directoryPath_;

	std::unique_ptr<Model> model_;

	std::unique_ptr<Animation> animation_;

};
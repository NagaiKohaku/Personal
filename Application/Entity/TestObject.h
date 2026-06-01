#pragma once

#include <Object/Base/GameObject.h>

class TestObject : public MyEngine::GameObject {

public:

	void Initialize() override;

	void Update() override;

	void Draw() override;
};
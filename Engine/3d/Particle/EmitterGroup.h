#pragma once

#include "ParticleEmitter.h"

#include "list"

class EmitterGroup {

public:

	void Initialize();

	void Update();

	void Draw();

private:

	//エミッターのリスト
	std::list<ParticleEmitter> particleEmitters_;
};
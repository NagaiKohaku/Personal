#include "EmitterGroup.h"

void EmitterGroup::Initialize() {
	//エミッターの初期化
	particleEmitters_.clear();
}

void EmitterGroup::Update() {
	//すべてのパーティクルグループの処理をする
	for (auto& emitter : particleEmitters_) {
		emitter.Update();
	}
}

void EmitterGroup::Draw() {
	//すべてのパーティクルグループの処理をする
	for (auto& emitter : particleEmitters_) {
		emitter.Draw(Object);
	}
}

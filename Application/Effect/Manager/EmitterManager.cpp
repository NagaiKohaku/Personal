#include <Effect/Manager/EmitterManager.h>

#include "fstream"
#include "filesystem"

using namespace MyEngine;

EmitterManager* EmitterManager::GetInstance() {
	static EmitterManager instance;
	return &instance;
}

void EmitterManager::Initialize() {

}

void EmitterManager::Update() {

	for (auto& emitter : emitters_) {

		if (isUpdate_) {

			emitter->object->Update();
		}
	}
}

void EmitterManager::Draw() {

	for (auto& emitter : emitters_) {

		emitter->object->Draw();
	}
}

EmitterGroup* EmitterManager::CreateEmitter(const std::string& emitterName, const std::string fileName, WorldTransform* parent) {

	std::unique_ptr<EmitterObject> newObject;

	newObject = std::make_unique<EmitterObject>();

	newObject->name = emitterName;
	newObject->object = std::make_unique<EmitterGroup>();
	newObject->object->Initialize(camera_);
	newObject->object->LoadEmitter(fileName);

	if (parent != nullptr) {

		newObject->object->GetWorldTransform().SetParent(parent);
	}

	emitters_.push_back(std::move(newObject));

	EmitterGroup* lastEmitter = emitters_.back().get()->object.get();

	return lastEmitter;
}

void EmitterManager::DeleteEmitter(EmitterGroup* emitterPtr) {

	for (auto it = emitters_.begin(); it != emitters_.end(); it++) {

		if (it->get()->object.get() == emitterPtr) {

			it = emitters_.erase(it);

			return;
		}
	}
}
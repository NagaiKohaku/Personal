#include <Object/Manager/ObjectManager.h>

#include <Object/Base/ObjectBase.h>
#include <Object/2D/Object2D.h>
#include <Object/3D/Object3D.h>

using namespace MyEngine;

ObjectManager* ObjectManager::GetInstance() {
	static ObjectManager instance;
	return &instance;
}

void ObjectManager::Initialize(
	Object2DCommon* object2DCommonPtr,
	Object3DCommon* object3DCommonPtr,
	Camera* cameraPtr,
	Renderer* rendererPtr
) {

	object2DCommon_ = object2DCommonPtr;
	object3DCommon_ = object3DCommonPtr;
	camera_ = cameraPtr;
	renderer_ = rendererPtr;
}

void ObjectManager::Update() {

	objects_.remove_if([](const std::unique_ptr<ObjectBase>& object) {
		if (object->GetIsRemove()) {
			return true;
		}
		return false;
		});
}

void ObjectManager::Draw() {

}

void ObjectManager::ClearAll() {

	for (auto& object : objects_) {

		object->SetIsRemove(true);
	}
}

Object2D* ObjectManager::CreateObject2D() {

	std::unique_ptr<Object2D> newObject;

	newObject->Initialize(object2DCommon_, camera_, renderer_);

	objects_.push_back(std::move(newObject));

	return static_cast<Object2D*>(objects_.back().get());
}

Object3D* ObjectManager::CreateObject3D() {

	std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>();

	newObject->Initialize(object3DCommon_, camera_, renderer_);

	objects_.push_back(std::move(newObject));

	return static_cast<Object3D*>(objects_.back().get());
}

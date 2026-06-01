#include "TestObject.h"

#include <Object/Component/Base/ComponentManager.h>

void TestObject::Initialize() {

	SetComponent(std::move(MyEngine::ComponentManager::GetInstance()->CreateWorldTransformComponent()));

	SetComponent(std::move(MyEngine::ComponentManager::GetInstance()->CreateModelComponent("Enemy")));

}

void TestObject::Update() {
}

void TestObject::Draw() {
}

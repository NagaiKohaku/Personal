#pragma once

#include <Object/Base/ObjectBase.h>
#include <Object/2D/Object2D.h>
#include <Object/2D/Object2DCommon.h>
#include <Object/3D/Object3D.h>
#include <Object/3D/Object3DCommon.h>
#include <3d/Camera/Camera.h>
#include <Base/Renderer.h>

#include <memory>
#include <list>

class ObjectManager {

public:

	static ObjectManager* GetInstance();

	void Initialize(
		MyEngine::Object2DCommon* object2DCommonPtr,
		MyEngine::Object3DCommon* object3DCommonPtr,
		MyEngine::Camera* cameraPtr,
		MyEngine::Renderer* rendererPtr
	);

	void Update();

	void Draw();

	void ClearAll();

	MyEngine::Object2D* CreateObject2D();

	MyEngine::Object3D* CreateObject3D();

private:

	std::list<std::unique_ptr<MyEngine::ObjectBase>> objects_;

	MyEngine::Object2DCommon* object2DCommon_;

	MyEngine::Object3DCommon* object3DCommon_;

	MyEngine::Camera* camera_;

	MyEngine::Renderer* renderer_;
};
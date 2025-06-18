#pragma once

#include "3d/Object/Object3D.h"
#include "3d/Particle/EmitterGroup.h"
#include "3d/Camera/Camera.h"

#include "Collider/AABBCollider.h"
#include "Collider/SphereCollider.h"

#include "memory"

class Enemy {

public:

	void Initialize();

	void Update();

	void Draw();

public:

	void SetCamera(Camera* ptr) {
		camera_ = ptr;
	}

private:

	void IsCollision();

private:

	Camera* camera_;

	std::unique_ptr<Object3D> enemy_ = nullptr;

	std::unique_ptr<SphereCollider> collider_ = nullptr;

};
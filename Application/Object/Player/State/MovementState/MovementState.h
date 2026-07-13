#pragma once

#include "Math/Vector/Vector3.h"

class Player;

class MovementState {

public:

	virtual ~MovementState() = default;

	virtual void Enter() = 0;

	virtual void Update(Player* player);

	virtual void CoreMoveUpdate();

	virtual void LeftWingMoveUpdate();

	virtual void RightWingMoveUpdate();

	MyEngine::Vector3 KnockBack();

protected:

	Player* player_;

	MyEngine::Vector3 initialPos_ = { 0.0f,2.0f,0.0f };

	MyEngine::Vector3 moveRange_ = { 7.0f,4.0f,0.0f };;

	MyEngine::Vector3 rotRange_ = { 0.0f,0.0f,0.0f };

	MyEngine::Vector3 leftWingTranslate_ = { 0.0f,0.0f,0.0f };

	MyEngine::Vector3 leftWingRotate_ = { 0.0f,0.0f,0.0f };

	MyEngine::Vector3 leftWingTrailPos_ = { 0.0f,0.0f,0.0f };

	MyEngine::Vector3 rightWingTranslate_ = { 0.0f,0.0f,0.0f };

	MyEngine::Vector3 rightWingRotate_ = { 0.0f,0.0f,0.0f };

	MyEngine::Vector3 rightWingTrailPos_ = { 0.0f,0.0f,0.0f };

	float moveSpeed_ = 1.0f;

	float moveStrength_ = 10.0f;

	float rotStrength_ = 10.0f;

	float stateChangeHeight_ = 1.0f;

	MyEngine::Vector3 movePos_ = { 0.0f,0.0f,0.0f };

	MyEngine::Vector3 moveRotate_ = { 0.0f,0.0f,0.0f };

	MyEngine::Vector3 knockBackPower_ = { 0.0f, 3.0f, -10.0f };

};
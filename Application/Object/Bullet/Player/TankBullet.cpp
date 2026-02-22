#include "TankBullet.h"

using namespace MyEngine;

///=====================================================/// 
/// 他オブジェクトと接触している場合の処理を行う
///=====================================================///
void TankBullet::IsCollision() {

	//接触状態であれば
	if (collider_->GetIsTrigger()) {

		//接触相手のタグがENEMYであれば
		if (collider_->CheckHitTag(Collider::Tag::ENEMY)) {

			isDead_ = true;
		}
	}
}
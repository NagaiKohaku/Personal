#include "ParticleManager.h"

#include "Base/DirectXCommon.h"
#include "Base/SrvManager.h"
#include "2d/Sprite/TextureManager.h"
#include "3d/Camera/Camera.h"
#include "3d/Particle/ParticleEmitter.h"
#include "3d/Particle/ParticleCommon.h"

#include "Math/MakeMatrixMath.h"

#include "Other/Log.h"

#include "random"
#include "numbers"

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
ParticleManager* ParticleManager::GetInstance() {
	static ParticleManager instance;
	return &instance;
}

///=====================================================/// 
/// 初期化処理
///=====================================================///
void ParticleManager::Initialize() {

	//DirectX基底のインスタンスを取得
	directXCommon_ = DirectXCommon::GetInstance();

	//テクスチャマネージャーのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();

	//SRVマネージャーのインスタンスを取得
	srvManager_ = SrvManager::GetInstance();

	//エミッターの最大数を設定
	particleEmitters.reserve(srvManager_->kMaxSRVCount_);
}

///=====================================================/// 
/// 更新処理
///=====================================================///
void ParticleManager::Update() {

	//すべてのパーティクルグループの処理をする
	for (std::unordered_map<std::string, ParticleEmitter>::iterator particleEmitterIterator = particleEmitters.begin();
		particleEmitterIterator != particleEmitters.end();) {

		//エミッターの更新
		particleEmitterIterator->second.Update();

		//次のエミッターへ移動
		++particleEmitterIterator;
	}

	//加速場との接触判定
	CheckCollisionAccelerationField();
}

///=====================================================/// 
/// 描画処理
///=====================================================///
void ParticleManager::Draw() {

	//パーティクルの描画前処理
	ParticleCommon::GetInstance()->CommonDrawSetting();

	//すべてのパーティクルグループの処理をする
	for (std::unordered_map<std::string, ParticleEmitter>::iterator particleEmitterIterator = particleEmitters.begin();
		particleEmitterIterator != particleEmitters.end();) {

		//エミッターの描画
		//particleEmitterIterator->second.Draw();

		//次のエミッターへ移動
		++particleEmitterIterator;
	}
}

void ParticleManager::CreateEmitter(const std::string name, const std::string textureFileName) {

	//登録済みの名前かチェック
	if (particleEmitters.contains(name)) {

		//登録済みなら早期return
		return;
	}

	ParticleEmitter& emitter = particleEmitters[name];

	//emitter.Initialize(name, textureFileName, defaultCamera_);
}

///=====================================================/// 
/// 発生
///=====================================================///
void ParticleManager::Emit(const std::string name, const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor, uint32_t count) {

	//登録済みの名前かチェック
	if (particleEmitters.contains(name)) {

		//エミッターからパーティクルを生成する
		particleEmitters[name].Emit();
	}
}

///=====================================================/// 
/// 加速場との接触判定
///=====================================================///
void ParticleManager::CheckCollisionAccelerationField() {

	//すべてのパーティクルグループの処理をする
	for (std::unordered_map<std::string, ParticleEmitter>::iterator particleEmitterIterator = particleEmitters.begin();
		particleEmitterIterator != particleEmitters.end();) {

		//加速場との接触判定
		particleEmitterIterator->second.CheckCollisionAccelerationField();

		//次のエミッターへ移動
		++particleEmitterIterator;
	}
}

///=====================================================/// 
/// 加速場のセッター
///=====================================================///
void ParticleManager::SetAcceleration(const std::string name, const Vector3& acceleration, const AABB& area) {

	//登録済みの名前かチェック
	if (particleEmitters.contains(name)) {

		//エミッターに加速場を設定する
		particleEmitters[name].SetAccelerationField(acceleration, area);
	}
}
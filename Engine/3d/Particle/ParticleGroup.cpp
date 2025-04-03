#include "ParticleGroup.h"

#include "Base/DirectXCommon.h"
#include "Base/SrvManager.h"
#include "2d/Sprite/TextureManager.h"
#include "3d/Camera/Camera.h"
#include "3d/Particle/ParticleCommon.h"

#include "Math/MakeMatrixMath.h"
#include "Math/Easing.h"

#include "random"
#include "numbers"

//インスタンスの最大数
const uint32_t ParticleGroup::kNumMaxInstance = 1000;

//1フレームで進む秒数
const float ParticleGroup::kDeltaTime = 1.0f / 60.0f;

void ParticleGroup::Initialize(const std::string textureFilePath, Camera* camera) {

	//DirectX基底のインスタンスを取得
	directXCommon_ = DirectXCommon::GetInstance();

	//テクスチャマネージャーのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();

	//SRVマネージャーのインスタンスを取得
	srvManager_ = SrvManager::GetInstance();

	//カメラ情報の設定
	defaultCamera_ = camera;

	//ランダムエンジンの初期化
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	randomEngine_ = randomEngine;

	material_.textureFilePath = textureFilePath;

	textureManager_->LoadTexture(material_.textureFilePath);

	material_.textureIndex = textureManager_->GetSrvIndex(material_.textureFilePath);

	//頂点リソースの生成
	vertexResource_ = directXCommon_->CreateBufferResource(sizeof(VertexData) * 4);

	//頂点インデックスリソースの生成
	IndexResource_ = directXCommon_->CreateBufferResource(sizeof(uint32_t) * 6);

	//マテリアルリソースの生成
	materialResource_ = directXCommon_->CreateBufferResource(sizeof(Material));

	instancingResource_ = directXCommon_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);

	//頂点バッファビューの作成
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	indexBufferView_.BufferLocation = IndexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;

	//1頂点当たりのサイズを設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//フォーマットを設定
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//頂点リソースに頂点データを書き込む
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	IndexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	instancingResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	srvIndex_ = srvManager_->Allocate();

	srvManager_->CreateSRVForStructuredBuffer(
		srvIndex_,
		instancingResource_.Get(),
		kNumMaxInstance,
		sizeof(ParticleForGPU)
	);

	//頂点データの初期化
	//左下
	vertexData_[0].position = { 0.0f,1.0f,0.0f,1.0f };
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[0].normal = { 0.0f,0.0f,-1.0f };
	//左上
	vertexData_[1].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[1].normal = { 0.0f,0.0f,-1.0f };
	//右下
	vertexData_[2].position = { 1.0f,1.0f,0.0f,1.0f };
	vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[2].normal = { 0.0f,0.0f,-1.0f };
	//右上
	vertexData_[3].position = { 1.0f,0.0f,0.0f,1.0f };
	vertexData_[3].texcoord = { 1.0f,0.0f };
	vertexData_[3].normal = { 0.0f,0.0f,-1.0f };

	//頂点インデックスデータの初期化
	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;

	//マテリアルデータの設定
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();

	for (uint32_t index = 0; index < kNumMaxInstance; index++) {
		instancingData_[index].WVP = MakeIdentity4x4();
		instancingData_[index].World = MakeIdentity4x4();
		instancingData_[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	srvIndex_ = srvManager_->Allocate();

	srvManager_->CreateSRVForStructuredBuffer(
		srvIndex_,
		instancingResource_.Get(),
		kNumMaxInstance,
		sizeof(ParticleForGPU)
	);

	useAccelerationField_ = false;
}

void ParticleGroup::Update() {

	//ビルボード行列の計算
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = backToFrontMatrix * defaultCamera_->GetWorldTransform().GetWorldMatrix();

	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	//カメラからビュープロジェクション行列を取得
	Matrix4x4 viewProjectionMatrix = defaultCamera_->GetViewProjectionMatrix();

	numInstance_ = 0;

	for (std::list<Particle>::iterator particleIterator = particles_.begin();
		particleIterator != particles_.end();) {

		if (particleIterator->lifeTime <= particleIterator->currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}

		particleIterator->transform.translate = particleIterator->transform.translate + particleIterator->velocity * kDeltaTime;

		particleIterator->currentTime += kDeltaTime;

		float alpha = Lerp(1.0f, 0.0f, particleIterator->currentTime / particleIterator->lifeTime);

		if (numInstance_ < kNumMaxInstance) {

			//拡縮行列の計算
			Matrix4x4 scaleMatrix = MakeScaleMatrix(particleIterator->transform.scale);

			//座標行列の計算
			Matrix4x4 translateMatrix = MakeTranslateMatrix(particleIterator->transform.translate);

			//ワールド行列の計算
			Matrix4x4 worldMatrix = scaleMatrix * (billboardMatrix * translateMatrix);

			//ワールドビュープロジェクション行列の合成
			Matrix4x4 worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;

			//インスタンシングデータに書き込む
			instancingData_[numInstance_].WVP = worldViewProjectionMatrix;
			instancingData_[numInstance_].World = worldMatrix;
			instancingData_[numInstance_].color = particleIterator->color;
			instancingData_[numInstance_].color.w = alpha;

			numInstance_++;
		}

		particleIterator++;
	}
}

void ParticleGroup::Draw() {

	//VBVを設定
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	directXCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(srvIndex_));

	directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(material_.textureIndex));

	directXCommon_->GetCommandList()->DrawIndexedInstanced(6, numInstance_, 0, 0, 0);

}

void ParticleGroup::Emit(const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor, uint32_t count) {

	std::list<Particle> particles;

	for (uint32_t i = 0; i < count; i++) {

		particles.push_back(MakeNewParticle(translate, area, minVelocity, maxVelocity, minTime, maxTime, useRandomColor));
	}

	particles_.splice(particles_.end(), particles);
}

void ParticleGroup::CheckCollisionAccelerationField() {

	if (useAccelerationField_) {

		for (std::list<Particle>::iterator particle = particles_.begin(); particle != particles_.end(); ++particle) {

			if (IsCollision(accelerationField_.area, particle->transform.translate)) {
				particle->velocity = particle->velocity + accelerationField_.acceleration * kDeltaTime;
			}
		}
	}
}

bool ParticleGroup::IsCollision(const AABB& aabb, const Vector3& point) {

	Vector3 closestPoint{
		std::clamp(point.x,aabb.min.x,aabb.max.x),
		std::clamp(point.y,aabb.min.y,aabb.max.y),
		std::clamp(point.z,aabb.min.z,aabb.max.z)
	};

	float distance = Length(closestPoint - point);

	if (distance <= 0.0f) {
		return true;
	}

	return false;
}

void ParticleGroup::SetAccelerationField(const Vector3& acceleration, const AABB& area) {

	accelerationField_.acceleration = acceleration;
	accelerationField_.area = area;

	useAccelerationField_ = true;
}

ParticleGroup::Particle ParticleGroup::MakeNewParticle(const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor) {

	Particle particle;

	std::uniform_real_distribution<float> distPosX(area.min.x, area.max.x);
	std::uniform_real_distribution<float> distPosY(area.min.y, area.max.y);
	std::uniform_real_distribution<float> distPosZ(area.min.z, area.max.z);

	std::uniform_real_distribution<float> distVelocityX(minVelocity.x, maxVelocity.x);
	std::uniform_real_distribution<float> distVelocityY(minVelocity.y, maxVelocity.y);
	std::uniform_real_distribution<float> distVelocityZ(minVelocity.z, maxVelocity.z);

	std::uniform_real_distribution<float> distColor(-1.0f, 1.0f);

	std::uniform_real_distribution<float> distTime(minTime, maxTime);

	particle.transform.scale = { 1.0f,1.0f,1.0f };
	particle.transform.rotate = { 0.0f,3.14f,0.0f };
	particle.transform.translate = { distPosX(randomEngine_),distPosY(randomEngine_) ,distPosZ(randomEngine_) };

	particle.velocity = { distVelocityX(randomEngine_),distVelocityY(randomEngine_),distVelocityZ(randomEngine_) };

	if (useRandomColor) {
		particle.color = { distColor(randomEngine_) ,distColor(randomEngine_) ,distColor(randomEngine_) ,1.0f };
	} else {
		particle.color = { 1.0f,1.0f,1.0f,1.0f };
	}

	particle.lifeTime = distTime(randomEngine_);
	particle.currentTime = 0.0f;

	particle.transform.translate = particle.transform.translate + translate;

	return particle;
}

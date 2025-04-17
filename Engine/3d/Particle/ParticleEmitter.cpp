#include "ParticleEmitter.h"

#include "Base/DirectXCommon.h"
#include "Base/SrvManager.h"
#include "2d/Sprite/TextureManager.h"
#include "3d/Camera/Camera.h"
#include "3d/Particle/ParticleCommon.h"
#include "3d/Particle/ParticleManager.h"

#include "Math/MakeMatrixMath.h"
#include "Math/Easing.h"
#include "Math/Random.h"

#include "numbers"

//インスタンスの最大数
const uint32_t ParticleEmitter::kNumMaxInstance = 1000;

//1フレームで進む秒数
const float ParticleEmitter::kDeltaTime = 1.0f / 60.0f;

void ParticleEmitter::Initialize(std::string name, const std::string textureFileName, Camera* camera) {

	//DirectX基底のインスタンスを取得
	directXCommon_ = DirectXCommon::GetInstance();

	//テクスチャマネージャーのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();

	//SRVマネージャーのインスタンスを取得
	srvManager_ = SrvManager::GetInstance();

	//パーティクルマネージャーのインスタンスを取得
	particleManager_ = ParticleManager::GetInstance();

	//カメラ情報の設定
	defaultCamera_ = camera;

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	worldTransform_.translate_ = { 0.0f,1.0f,0.0f };

	//加速場フラグの初期化
	useAccelerationField_ = false;

	//テクスチャパスを取得
	material_.textureFilePath = "Resource/Sprite/Particle/" + textureFileName;

	//テクスチャのロード
	textureManager_->LoadTexture(material_.textureFilePath);

	//テクスチャのSRVインデックスを取得
	material_.textureIndex = textureManager_->GetSrvIndex(material_.textureFilePath);

	/// === 頂点リソースの生成 === ///

	//頂点リソースの生成
	vertexResource_ = directXCommon_->CreateBufferResource(sizeof(VertexData) * 4);

	//頂点バッファビューの作成
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;

	//1頂点当たりのサイズを設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//リソースにデータを書き込めるようにする
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//頂点データの初期化
	//左下
	vertexData_[0].position = { -0.5f,0.5f,0.0f,1.0f };
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[0].normal = { 0.0f,0.0f,-1.0f };
	//左上
	vertexData_[1].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[1].normal = { 0.0f,0.0f,-1.0f };
	//右下
	vertexData_[2].position = { 0.5f,0.5f,0.0f,1.0f };
	vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[2].normal = { 0.0f,0.0f,-1.0f };
	//右上
	vertexData_[3].position = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData_[3].texcoord = { 1.0f,0.0f };
	vertexData_[3].normal = { 0.0f,0.0f,-1.0f };

	/// === 頂点インデックスリソースの生成 === ///

	//頂点インデックスリソースの生成
	IndexResource_ = directXCommon_->CreateBufferResource(sizeof(uint32_t) * 6);

	//リソースの場所を取得
	indexBufferView_.BufferLocation = IndexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;

	//フォーマットを設定
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//リソースにデータを書き込めるようにする
	IndexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	//頂点インデックスデータの初期化
	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;

	/// === マテリアルリソースの生成 === ///

	//マテリアルリソースの生成
	materialResource_ = directXCommon_->CreateBufferResource(sizeof(Material));

	//リソースにデータを書き込めるようにする
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	//マテリアルデータの設定
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();

	/// === インスタンシングリソースの生成 === ///

	//インスタンシングリソースの生成
	instancingResource_ = directXCommon_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);

	//リソースにデータを書き込めるようにする
	instancingResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	//インスタンシングデータの初期化
	for (uint32_t index = 0; index < kNumMaxInstance; index++) {
		instancingData_[index].WVP = MakeIdentity4x4();
		instancingData_[index].World = MakeIdentity4x4();
		instancingData_[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//SRVインデックスを取得
	srvIndex_ = srvManager_->Allocate();

	//SRVを生成
	srvManager_->CreateSRVForStructuredBuffer(
		srvIndex_,
		instancingResource_.Get(),
		kNumMaxInstance,
		sizeof(ParticleForGPU)
	);

	//particleManager_->AddEmitter(name, *this);
}

void ParticleEmitter::Update() {

	//ビルボード行列の計算
	Matrix4x4 billboardMatrix = defaultCamera_->GetViewMatrix();

	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;
	billboardMatrix.m[3][3] = 1.0f;

	billboardMatrix = Inverse4x4(billboardMatrix);

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

			// 回転行列
			Matrix4x4 rotateXMatrix = MakeRotateXMatrix(particleIterator->transform.rotate.x);
			Matrix4x4 rotateYMatrix = MakeRotateYMatrix(particleIterator->transform.rotate.y);
			Matrix4x4 rotateZMatrix = MakeRotateZMatrix(particleIterator->transform.rotate.z);
			Matrix4x4 rotateMatrix = rotateXMatrix * (rotateYMatrix * rotateZMatrix);

			//座標行列の計算
			Matrix4x4 translateMatrix = MakeTranslateMatrix(particleIterator->transform.translate);

			//ワールド行列の計算
			Matrix4x4 worldMatrix = scaleMatrix * (rotateMatrix * translateMatrix);

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

void ParticleEmitter::Draw() {

	//VBVを設定
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	directXCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(srvIndex_));

	directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(material_.textureIndex));

	directXCommon_->GetCommandList()->DrawIndexedInstanced(6, numInstance_, 0, 0, 0);

}

void ParticleEmitter::Emit(const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor, uint32_t count) {

	std::list<Particle> particles;

	for (uint32_t i = 0; i < count; i++) {

		particles.push_back(MakeNewParticle(translate, area, minVelocity, maxVelocity, minTime, maxTime, useRandomColor));
	}

	particles_.splice(particles_.end(), particles);

}

void ParticleEmitter::EmitPlane(const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor, uint32_t count) {

	std::list<Particle> particles;

	for (uint32_t i = 0; i < count; i++) {

		particles.push_back(MakeNewPlaneParticle(translate, area, minVelocity, maxVelocity, minTime, maxTime, useRandomColor));
	}

	particles_.splice(particles_.end(), particles);

}

void ParticleEmitter::CheckCollisionAccelerationField() {

	if (useAccelerationField_) {

		for (std::list<Particle>::iterator particle = particles_.begin(); particle != particles_.end(); ++particle) {

			if (IsCollision(accelerationField_.area, particle->transform.translate)) {
				particle->velocity = particle->velocity + accelerationField_.acceleration * kDeltaTime;
			}
		}
	}
}

bool ParticleEmitter::IsCollision(const AABB& aabb, const Vector3& point) {

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

void ParticleEmitter::SetAccelerationField(const Vector3& acceleration, const AABB& area) {

	accelerationField_.acceleration = acceleration;
	accelerationField_.area = area;

	useAccelerationField_ = true;
}

ParticleEmitter::Particle ParticleEmitter::MakeNewParticle(const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor) {

	//新しいパーティクルの生成
	Particle particle;

	particle.transform.scale = { 1.0f,1.0f,1.0f };
	particle.transform.rotate = { 0.0f,3.14f,0.0f };
	particle.transform.translate = { RandomVector3(area.min,area.max) };

	particle.velocity = { RandomVector3(minVelocity,maxVelocity) };

	particle.color = { 1.0f,1.0f,1.0f,1.0f };

	particle.lifeTime = RandomFloat(minTime, maxTime);
	particle.currentTime = 0.0f;

	particle.transform.translate = particle.transform.translate + translate;

	return particle;
}

ParticleEmitter::Particle ParticleEmitter::MakeNewPlaneParticle(const Vector3& translate, const AABB& area, const Vector3& minVelocity, const Vector3& maxVelocity, float minTime, float maxTime, bool useRandomColor) {

	Particle particle;

	particle.transform.scale = { 0.05f,1.0f,1.0f };
	particle.transform.rotate = { RandomVector3({0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}) };
	particle.transform.translate = { 0.0f,0.0f,0.0f };

	particle.velocity = { RandomVector3(minVelocity,maxVelocity) };

	particle.color = { 1.0f,1.0f,1.0f,1.0f };

	particle.lifeTime = RandomFloat(minTime, maxTime);
	particle.currentTime = 0.0f;

	particle.transform.translate = particle.transform.translate + worldTransform_.translate_;

	return particle;
}

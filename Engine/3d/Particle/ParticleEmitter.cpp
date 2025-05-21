#include "ParticleEmitter.h"

#include "Base/DirectXCommon.h"
#include "Base/SrvManager.h"
#include "Base/Renderer.h"
#include "Base//Input.h"
#include "2d/Sprite/TextureManager.h"
#include "3d/Camera/Camera.h"
#include "3d/Particle/ParticleCommon.h"
#include "3d/Particle/ParticleManager.h"
#include "3d/Primitive/Plane.h"
#include "3d/Primitive/Ring.h"
#include "3d/Primitive/Cylinder.h"

#include "Math/MakeMatrixMath.h"
#include "Math/Easing.h"
#include "Math/Random.h"

#include "imgui.h"

#include "json.hpp"
#include "fstream"
#include "numbers"

//インスタンスの最大数
const uint32_t ParticleEmitter::kNumMaxInstance = 1000;

//1フレームで進む秒数
const float ParticleEmitter::kDeltaTime = 1.0f / 60.0f;

void ParticleEmitter::Initialize(const std::string& fileName, Camera* camera) {

	/// === シングルトンインスタンスの取得 === ///

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

	/// === エミッター情報の初期化 === ///

	directoryPath_ = "Resource/Json/Particle/Emitter/";

	//ワールドトランスフォームの初期化
	emitterWorldTransform_.Initialize();

	ImportEmitterData(fileName);

	emitTimer_ = 0.0f;

	emitCount_ = 0;

	isActive_ = false;

	//加速場フラグの初期化
	useAccelerationField_ = false;

	/// === パーティクル情報の初期化 === ///

	//プリミティブの生成
	primitive_ = CreatePrimitive(primitiveType_);

	primitive_->Initialize();

	//テクスチャパスを取得
	material_.textureFilePath = "Resource/Sprite/Particle/" + textureFileName_;

	//テクスチャのロード
	textureManager_->LoadTexture(material_.textureFilePath);

	//テクスチャのSRVインデックスを取得
	material_.textureIndex = textureManager_->GetSrvIndex(material_.textureFilePath);
}

void ParticleEmitter::Update() {

	emitterWorldTransform_.UpdateMatrix();

	emitTimer_ += 1.0f / 60.0f;

	uvTimer_ += 1.0f / 60.0f;

	Vector3 uvTransform = { uvTimer_,0.0f,0.0f };
	Matrix4x4 uvTransformMatrix = MakeTranslateMatrix(uvTransform);
	Matrix4x4 uvScaleMatrix = MakeIdentity4x4();
	Matrix4x4 uvRotateMatrix = MakeIdentity4x4();

	materialData_->uvTransform = (uvScaleMatrix * uvRotateMatrix) * uvTransformMatrix;

	if (isActive_) {

		std::list<Particle> particles;

		while (emitTimer_ >= emitFrequency_) {

			if (emitCount_ >= emitMaxCount_) {

				isActive_ = false;

				break;
			}

			particles.push_back(MakeNewParticle());

			emitTimer_ -= emitFrequency_;

			emitCount_++;
		}

		particles_.splice(particles_.end(), particles);
	}

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

	for (std::list<Particle>::iterator particle = particles_.begin();
		particle != particles_.end();) {

		if (particle->lifeTime <= particle->currentTime) {
			particle = particles_.erase(particle);
			continue;
		}

		if (numInstance_ < kNumMaxInstance) {

			/// === 座標の計算 === ///

			particle->positionPara.velocity = particle->positionPara.velocity + particle->positionPara.acceleration * kDeltaTime;

			particle->transform.translate_ = particle->transform.translate_ + particle->positionPara.velocity * kDeltaTime;

			/// === 回転角の計算 === ///

			particle->rotationPara.velocity = particle->rotationPara.velocity + particle->rotationPara.acceleration * kDeltaTime;

			particle->transform.rotate_ = particle->transform.rotate_ + particle->rotationPara.velocity * kDeltaTime;

			/// === 拡縮の計算 === ///

			particle->scalePara.velocity = particle->scalePara.velocity + particle->scalePara.acceleration * kDeltaTime;

			particle->transform.scale_ = particle->transform.scale_ + particle->scalePara.velocity * kDeltaTime;

			/// === 色の計算 === ///

			particle->colorPara.velocity = particle->colorPara.velocity + particle->colorPara.acceleration * kDeltaTime;

			//particle->color = particle->color + particle->colorPara.velocity * kDeltaTime;

			particle->color = Lerp(
				particle->colorPara.startColor,
				particle->colorPara.endColor,
				particle->currentTime / particle->lifeTime
			);

			particle->currentTime += kDeltaTime;

			particle->transform.UpdateMatrix();

			Matrix4x4 translateEMatrix = emitterWorldTransform_.GetTranslateMatrix();
			Matrix4x4 rotateEMatrix = emitterWorldTransform_.GetRotateMatrix();
			Matrix4x4 scaleEMatrix = emitterWorldTransform_.GetScaleMatrix();

			Matrix4x4 translateMatrix = particle->transform.GetTranslateMatrix();
			Matrix4x4 rotateMatrix = particle->transform.GetRotateMatrix();
			Matrix4x4 scaleMatrix = particle->transform.GetScaleMatrix();

			translateMatrix = translateMatrix * translateEMatrix;
			rotateMatrix = rotateMatrix * rotateEMatrix;
			scaleMatrix = scaleMatrix * scaleEMatrix;

			if (isBillboard_) {

				rotateMatrix = billboardMatrix;
			}

			//ワールド行列の計算
			Matrix4x4 worldMatrix = (scaleMatrix * rotateMatrix) * translateMatrix;

			//ワールドビュープロジェクション行列の合成
			Matrix4x4 worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;

			//インスタンシングデータに書き込む
			instancingData_[numInstance_].WVP = worldViewProjectionMatrix;
			instancingData_[numInstance_].World = worldMatrix;
			instancingData_[numInstance_].color = particle->color;
			instancingData_[numInstance_].color.w = particle->color.w;

			numInstance_++;
		}

		particle++;
	}
}

void ParticleEmitter::Draw(LayerType layer) {

	//Renderクラスに渡す
	std::function<void()> command;

	command = [this]() {

		//パーティクルの描画前処理
		ParticleCommon::GetInstance()->CommonDrawSetting();

		primitive_->Draw();

		directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

		directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(srvIndex_));

		directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(material_.textureIndex));

		directXCommon_->GetCommandList()->DrawIndexedInstanced(primitive_->GetIndexCount(), numInstance_, 0, 0, 0);
		};

	Renderer::GetInstance()->AddDraw(layer, command);
}

void ParticleEmitter::ImGui() {

	std::string currentName = name_;

	if (ImGui::CollapsingHeader("EmitterSettiing")) {
		ImGui::Columns(2, "EmitterColumns", false);

		ImGui::Text("Name");
		if (ImGui::InputText("##Name", currentName.data(), 256)) {
			if (Input::GetInstance()->IsTriggerPushKey(DIK_RETURN)) {
				name_ = currentName.c_str();
			}
		}
		ImGui::NextColumn();

		const char* primitiveItems[] = { "Plane","Ring","Cylinder" };

		int currentPrimitive = static_cast<int>(primitiveType_);

		ImGui::Text("Primitive");
		if (ImGui::Combo("##Primitive", &currentPrimitive, primitiveItems, IM_ARRAYSIZE(primitiveItems))) {

			primitiveType_ = static_cast<PrimitiveType>(currentPrimitive);

			primitive_.reset();

			primitive_ = CreatePrimitive(primitiveType_);

			primitive_->Initialize();
		}
		ImGui::NextColumn();

		std::vector<const char*> textureItems;

		int currentTexture = 0;

		for (auto& textureName : textureList_) {

			textureItems.push_back(textureName.c_str());
		}

		textureItems.insert(textureItems.begin(), "Select Texture");

		ImGui::Text("Texture");
		if (ImGui::Combo("##Texture", &currentTexture, textureItems.data(), static_cast<int>(textureItems.size()))) {

			textureFileName_ = textureItems[currentTexture];

			material_.textureFilePath = "Resource/Sprite/Particle/" + textureFileName_;

			material_.textureIndex = textureManager_->GetSrvIndex(material_.textureFilePath);
		}

		ImGui::Text("EmitterPosition");
			ImGui::DragFloat3("##EmitterPosition", &emitterWorldTransform_.translate_.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("EmitterRotation");
			ImGui::DragFloat3("##EmitterRotation", &emitterWorldTransform_.rotate_.x, 0.01f);
			ImGui::NextColumn();

			ImGui::Text("EmitterScale");
		ImGui::DragFloat3("##EmitterScale", &emitterWorldTransform_.scale_.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("EmitCount");
		ImGui::InputInt("##EmitCount", &emitMaxCount_);
		ImGui::NextColumn();

		ImGui::Text("frequency");
		ImGui::DragFloat("##frequency", &emitFrequency_, 0.01f);
		ImGui::NextColumn();

		ImGui::Text("lifeTime");
		ImGui::DragFloat("##lifeTime", &particleLifeTime_, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("lifeTimeRandomRange");
		ImGui::DragFloat("##lifeTimeRandomRange", &particleLifeTimeRandomRange_, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("isLoop");
		ImGui::Checkbox("##isLoop", &isLoop_);
		ImGui::NextColumn();

		ImGui::Text("isBillboard");
		ImGui::Checkbox("##isBillboard", &isBillboard_);
		ImGui::NextColumn();

		ImGui::Columns(1);
	}

	if (ImGui::CollapsingHeader("Position")) {
		ImGui::Columns(2, "PositionColumns", false);

		ImGui::Text("StartNum");
		ImGui::DragFloat3("##PositionStartNum", &positionParameter_.startNum.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("StartRandomRange");
		ImGui::DragFloat3("##PositionStartRandomRange", &positionParameter_.startRandomRange.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("EndNum");
		ImGui::DragFloat3("##PositionEndNum", &positionParameter_.endNum.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("EndRandomRange");
		ImGui::DragFloat3("##PositionEndRandomRange", &positionParameter_.endRandomRange.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("Velocity");
		ImGui::DragFloat3("##PositionVelocity", &positionParameter_.velocity.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("VelocityRandomRange");
		ImGui::DragFloat3("##PositionVelocityRandomRange", &positionParameter_.velocityRandomRange.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("Acceleration");
		ImGui::DragFloat3("##PositionAcceleration", &positionParameter_.acceleration.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("AccelerationRandomRange");
		ImGui::DragFloat3("##PositionAccelerationRandomRange", &positionParameter_.accelerationRandomRange.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Columns(1);
	}

	if (ImGui::CollapsingHeader("Rotation")) {
		ImGui::Columns(2, "RotationColumns", false);

		ImGui::Text("StartNum");
		ImGui::DragFloat3("##StartNum", &rotationParameter_.startNum.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("StartRandomRange");
		ImGui::DragFloat3("##StartRandomRange", &rotationParameter_.startRandomRange.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("EndNum");
		ImGui::DragFloat3("##EndNum", &rotationParameter_.endNum.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("EndRandomRange");
		ImGui::DragFloat3("##EndRandomRange", &rotationParameter_.endRandomRange.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("Velocity");
		ImGui::DragFloat3("##Velocity", &rotationParameter_.velocity.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("VelocityRandomRange");
		ImGui::DragFloat3("##VelocityRandomRange", &rotationParameter_.velocityRandomRange.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("Acceleration");
		ImGui::DragFloat3("##Acceleration", &rotationParameter_.acceleration.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("AccelerationRandomRange");
		ImGui::DragFloat3("##AccelerationRandomRange", &rotationParameter_.accelerationRandomRange.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Columns(1);
	}

	if (ImGui::CollapsingHeader("Scale")) {
		ImGui::Columns(2, "ScaleColumns", false);

		ImGui::Text("StartNum");
		ImGui::DragFloat3("##StartNum", &scaleParameter_.startNum.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("StartRandomRange");
		ImGui::DragFloat3("##StartRandomRange", &scaleParameter_.startRandomRange.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("EndNum");
		ImGui::DragFloat3("##EndNum", &scaleParameter_.endNum.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("EndRandomRange");
		ImGui::DragFloat3("##EndRandomRange", &scaleParameter_.endRandomRange.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("Velocity");
		ImGui::DragFloat3("##Velocity", &scaleParameter_.velocity.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("VelocityRandomRange");
		ImGui::DragFloat3("##VelocityRandomRange", &scaleParameter_.velocityRandomRange.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("Acceleration");
		ImGui::DragFloat3("##Acceleration", &scaleParameter_.acceleration.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Text("AccelerationRandomRange");
		ImGui::DragFloat3("##AccelerationRandomRange", &scaleParameter_.accelerationRandomRange.x, 0.1f);
		ImGui::NextColumn();

		ImGui::Columns(1);
	}

	if (ImGui::CollapsingHeader("Color")) {
		ImGui::Columns(2, "ColorColumns", false);

		ImGui::Text("StartColor");
		ImGui::ColorEdit4("##StartColor", &colorParameter_.startColor.x);
		ImGui::NextColumn();

		ImGui::Text("StartColorRandomRange");
		ImGui::ColorEdit4("##StartColorRandomRange", &colorParameter_.startRandomRange.x);
		ImGui::NextColumn();

		ImGui::Text("EndColor");
		ImGui::ColorEdit4("##EndColor", &colorParameter_.endColor.x);
		ImGui::NextColumn();

		ImGui::Text("EndColorRandomRange");
		ImGui::ColorEdit4("##EndColorRandomRange", &colorParameter_.endRandomRange.x);
		ImGui::NextColumn();

		ImGui::Text("Velocity");
		ImGui::ColorEdit4("##Velocity", &colorParameter_.velocity.x);
		ImGui::NextColumn();

		ImGui::Text("VelocityRandomRange");
		ImGui::ColorEdit4("##VelocityRandomRange", &colorParameter_.velocityRandomRange.x);
		ImGui::NextColumn();

		ImGui::Text("Acceleration");
		ImGui::ColorEdit4("##Acceleration", &colorParameter_.acceleration.x);
		ImGui::NextColumn();

		ImGui::Text("AccelerationRandomRange");
		ImGui::ColorEdit4("##AccelerationRandomRange", &colorParameter_.accelerationRandomRange.x);
		ImGui::NextColumn();

		ImGui::Columns(1);
	}

	if (ImGui::Button("Emit")) {
		Emit();
	}

}

void ParticleEmitter::Emit() {

	isActive_ = true;

	emitCount_ = 0;

	emitTimer_ = 0;
}

void ParticleEmitter::CheckCollisionAccelerationField() {

	if (useAccelerationField_) {

		for (std::list<Particle>::iterator particle = particles_.begin(); particle != particles_.end(); ++particle) {

			if (IsCollision(accelerationField_.area, particle->transform.translate_)) {
				particle->positionPara.velocity = particle->positionPara.velocity + accelerationField_.acceleration * kDeltaTime;
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

void ParticleEmitter::ExportEmitterData(const std::string& fileName) {

	nlohmann::json jsonData;

	std::string filePath = directoryPath_ + fileName + ".json";

	jsonData["name"] = name_;

	jsonData["textureFileName"] = textureFileName_;

	jsonData["primitiveType"] = primitiveType_;

	jsonData["lifeTime"] = particleLifeTime_;
	jsonData["lifeTimeRandomRange"] = particleLifeTimeRandomRange_;
	jsonData["frequency"] = emitFrequency_;
	jsonData["maxCount"] = emitMaxCount_;

	jsonData["position"] = {
		{"startNum", {positionParameter_.startNum.x,positionParameter_.startNum.y,positionParameter_.startNum.z}},
		{"startRandomRange", {positionParameter_.startRandomRange.x,positionParameter_.startRandomRange.y,positionParameter_.startRandomRange.z}},
		{"endNum", {positionParameter_.endNum.x,positionParameter_.endNum.y,positionParameter_.endNum.z}},
		{"endRandomRange", {positionParameter_.endRandomRange.x,positionParameter_.endRandomRange.y,positionParameter_.endRandomRange.z}},
		{"velocity", {positionParameter_.velocity.x,positionParameter_.velocity.y,positionParameter_.velocity.z}},
		{"velocityRandomRange", {positionParameter_.velocityRandomRange.x,positionParameter_.velocityRandomRange.y,positionParameter_.velocityRandomRange.z}},
		{"acceleration", {positionParameter_.acceleration.x,positionParameter_.acceleration.y,positionParameter_.acceleration.z}},
		{"accelerationRandomRange", {positionParameter_.accelerationRandomRange.x,positionParameter_.accelerationRandomRange.y,positionParameter_.accelerationRandomRange.z}}
	};

	jsonData["rotation"] = {
		{"startNum", {rotationParameter_.startNum.x,rotationParameter_.startNum.y,rotationParameter_.startNum.z}},
		{"startRandomRange", {rotationParameter_.startRandomRange.x,rotationParameter_.startRandomRange.y,rotationParameter_.startRandomRange.z}},
		{"endNum", {rotationParameter_.endNum.x,rotationParameter_.endNum.y,rotationParameter_.endNum.z}},
		{"endRandomRange", {rotationParameter_.endRandomRange.x,rotationParameter_.endRandomRange.y,rotationParameter_.endRandomRange.z}},
		{"velocity", {rotationParameter_.velocity.x,rotationParameter_.velocity.y,rotationParameter_.velocity.z}},
		{"velocityRandomRange", {rotationParameter_.velocityRandomRange.x,rotationParameter_.velocityRandomRange.y,rotationParameter_.velocityRandomRange.z}},
		{"acceleration", {rotationParameter_.acceleration.x,rotationParameter_.acceleration.y,rotationParameter_.acceleration.z}},
		{"accelerationRandomRange", {rotationParameter_.accelerationRandomRange.x,rotationParameter_.accelerationRandomRange.y,rotationParameter_.accelerationRandomRange.z}}
	};

	jsonData["scale"] = {
		{"startNum", {scaleParameter_.startNum.x,scaleParameter_.startNum.y,scaleParameter_.startNum.z}},
		{"startRandomRange", {scaleParameter_.startRandomRange.x,scaleParameter_.startRandomRange.y,scaleParameter_.startRandomRange.z}},
		{"endNum", {scaleParameter_.endNum.x,scaleParameter_.endNum.y,scaleParameter_.endNum.z}},
		{"endRandomRange", {scaleParameter_.endRandomRange.x,scaleParameter_.endRandomRange.y,scaleParameter_.endRandomRange.z}},
		{"velocity", {scaleParameter_.velocity.x,scaleParameter_.velocity.y,scaleParameter_.velocity.z}},
		{"velocityRandomRange", {scaleParameter_.velocityRandomRange.x,scaleParameter_.velocityRandomRange.y,scaleParameter_.velocityRandomRange.z}},
		{"acceleration", {scaleParameter_.acceleration.x,scaleParameter_.acceleration.y,scaleParameter_.acceleration.z}},
		{"accelerationRandomRange", {scaleParameter_.accelerationRandomRange.x,scaleParameter_.accelerationRandomRange.y,scaleParameter_.accelerationRandomRange.z}}
	};

	jsonData["color"] = {
		{"startColor", {colorParameter_.startColor.x,colorParameter_.startColor.y,colorParameter_.startColor.z,colorParameter_.startColor.w}},
		{"startRandomRange", {colorParameter_.startRandomRange.x,colorParameter_.startRandomRange.y,colorParameter_.startRandomRange.z,colorParameter_.startRandomRange.w}},
		{"endColor", {colorParameter_.endColor.x,colorParameter_.endColor.y,colorParameter_.endColor.z,colorParameter_.endColor.w}},
		{"endRandomRange", {colorParameter_.endRandomRange.x,colorParameter_.endRandomRange.y,colorParameter_.endRandomRange.z,colorParameter_.endRandomRange.w}},
		{"velocity", {colorParameter_.velocity.x,colorParameter_.velocity.y,colorParameter_.velocity.z,colorParameter_.velocity.w}},
		{"velocityRandomRange", {colorParameter_.velocityRandomRange.x,colorParameter_.velocityRandomRange.y,colorParameter_.velocityRandomRange.z,colorParameter_.velocityRandomRange.w}},
		{"acceleration", {colorParameter_.acceleration.x,colorParameter_.acceleration.y,colorParameter_.acceleration.z,colorParameter_.acceleration.w}},
		{"accelerationRandomRange", {colorParameter_.accelerationRandomRange.x,colorParameter_.accelerationRandomRange.y,colorParameter_.accelerationRandomRange.z,colorParameter_.accelerationRandomRange.w}}
	};

	std::filesystem::path dir(directoryPath_);
	if (!std::filesystem::exists(directoryPath_)) {
		std::filesystem::create_directory(directoryPath_);
	}

	std::ofstream file;

	file.open(filePath);

	if (file.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "ParticleEmitter", 0);
		assert(0);
		return;
	}

	file << jsonData.dump(4);

	file.close();
}

void ParticleEmitter::ImportEmitterData(const std::string& fileName) {

	nlohmann::json jsonData;

	std::string filePath = directoryPath_ + fileName + ".json";

	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::string message = "Failed open data file for read.";
		MessageBoxA(nullptr, message.c_str(), "ParticleEmitter", 0);
		assert(0);
		return;
	}

	file >> jsonData;

	file.close();

	name_ = jsonData["name"];

	textureFileName_ = jsonData["textureFileName"];

	primitiveType_ = static_cast<PrimitiveType>(jsonData["primitiveType"]);

	particleLifeTime_ = jsonData["lifeTime"];
	particleLifeTimeRandomRange_ = jsonData["lifeTimeRandomRange"];
	emitFrequency_ = jsonData["frequency"];
	emitMaxCount_ = jsonData["maxCount"];

	if (jsonData.contains("position")) {

		auto position = jsonData["position"];
		positionParameter_.startNum = { position["startNum"][0],position["startNum"][1],position["startNum"][2] };
		positionParameter_.startRandomRange = { position["startRandomRange"][0],position["startRandomRange"][1],position["startRandomRange"][2] };
		positionParameter_.endNum = { position["endNum"][0],position["endNum"][1],position["endNum"][2] };
		positionParameter_.endRandomRange = { position["endRandomRange"][0],position["endRandomRange"][1],position["endRandomRange"][2] };
		positionParameter_.velocity = { position["velocity"][0],position["velocity"][1],position["velocity"][2] };
		positionParameter_.velocityRandomRange = { position["velocityRandomRange"][0],position["velocityRandomRange"][1],position["velocityRandomRange"][2] };
		positionParameter_.acceleration = { position["acceleration"][0],position["acceleration"][1],position["acceleration"][2] };
		positionParameter_.accelerationRandomRange = { position["accelerationRandomRange"][0],position["accelerationRandomRange"][1],position["accelerationRandomRange"][2] };
	}

	if (jsonData.contains("rotation")) {
		auto rotation = jsonData["rotation"];
		rotationParameter_.startNum = { rotation["startNum"][0],rotation["startNum"][1],rotation["startNum"][2] };
		rotationParameter_.startRandomRange = { rotation["startRandomRange"][0],rotation["startRandomRange"][1],rotation["startRandomRange"][2] };
		rotationParameter_.endNum = { rotation["endNum"][0],rotation["endNum"][1],rotation["endNum"][2] };
		rotationParameter_.endRandomRange = { rotation["endRandomRange"][0],rotation["endRandomRange"][1],rotation["endRandomRange"][2] };
		rotationParameter_.velocity = { rotation["velocity"][0],rotation["velocity"][1],rotation["velocity"][2] };
		rotationParameter_.velocityRandomRange = { rotation["velocityRandomRange"][0],rotation["velocityRandomRange"][1],rotation["velocityRandomRange"][2] };
		rotationParameter_.acceleration = { rotation["acceleration"][0],rotation["acceleration"][1],rotation["acceleration"][2] };
		rotationParameter_.accelerationRandomRange = { rotation["accelerationRandomRange"][0],rotation["accelerationRandomRange"][1],rotation["accelerationRandomRange"][2] };
	}

	if (jsonData.contains("scale")) {
		auto scale = jsonData["scale"];
		scaleParameter_.startNum = { scale["startNum"][0],scale["startNum"][1],scale["startNum"][2] };
		scaleParameter_.startRandomRange = { scale["startRandomRange"][0],scale["startRandomRange"][1],scale["startRandomRange"][2] };
		scaleParameter_.endNum = { scale["endNum"][0],scale["endNum"][1],scale["endNum"][2] };
		scaleParameter_.endRandomRange = { scale["endRandomRange"][0],scale["endRandomRange"][1],scale["endRandomRange"][2] };
		scaleParameter_.velocity = { scale["velocity"][0],scale["velocity"][1],scale["velocity"][2] };
		scaleParameter_.velocityRandomRange = { scale["velocityRandomRange"][0],scale["velocityRandomRange"][1],scale["velocityRandomRange"][2] };
		scaleParameter_.acceleration = { scale["acceleration"][0],scale["acceleration"][1],scale["acceleration"][2] };
		scaleParameter_.accelerationRandomRange = { scale["accelerationRandomRange"][0],scale["accelerationRandomRange"][1],scale["accelerationRandomRange"][2] };
	}

	if (jsonData.contains("color")) {
		auto color = jsonData["color"];
		colorParameter_.startColor = { color["startColor"][0],color["startColor"][1],color["startColor"][2],color["startColor"][3] };
		colorParameter_.startRandomRange = { color["startRandomRange"][0],color["startRandomRange"][1],color["startRandomRange"][2],color["startRandomRange"][3] };
		colorParameter_.endColor = { color["endColor"][0],color["endColor"][1],color["endColor"][2],color["endColor"][3] };
		colorParameter_.endRandomRange = { color["endRandomRange"][0],color["endRandomRange"][1],color["endRandomRange"][2],color["endRandomRange"][3] };
		colorParameter_.velocity = { color["velocity"][0],color["velocity"][1],color["velocity"][2],color["velocity"][3] };
		colorParameter_.velocityRandomRange = { color["velocityRandomRange"][0],color["velocityRandomRange"][1],color["velocityRandomRange"][2],color["velocityRandomRange"][3] };
		colorParameter_.acceleration = { color["acceleration"][0],color["acceleration"][1],color["acceleration"][2],color["acceleration"][3] };
		colorParameter_.accelerationRandomRange = { color["accelerationRandomRange"][0],color["accelerationRandomRange"][1],color["accelerationRandomRange"][2],color["accelerationRandomRange"][3] };
	}
}

void ParticleEmitter::SetAccelerationField(const Vector3& acceleration, const AABB& area) {

	accelerationField_.acceleration = acceleration;
	accelerationField_.area = area;

	useAccelerationField_ = true;
}

std::unique_ptr<PrimitiveBase> ParticleEmitter::CreatePrimitive(PrimitiveType primitiveType) {

	switch (primitiveType) {
	case ParticleEmitter::PLANE:
		return std::move(std::make_unique<Plane>());
		break;
	case ParticleEmitter::RING:
		return std::move(std::make_unique<Ring>());
		break;
	case ParticleEmitter::CYLINDER:
		return std::move(std::make_unique<Cylinder>());
		break;
	}

	return std::move(std::make_unique<Plane>());
}

ParticleEmitter::Particle ParticleEmitter::MakeNewParticle() {

	//新しいパーティクルの生成
	Particle particle;

	particle.positionPara = {
		RandomRangeVector3(positionParameter_.startNum, positionParameter_.startRandomRange),
		RandomRangeVector3(positionParameter_.endNum, positionParameter_.endRandomRange),
		RandomRangeVector3(positionParameter_.velocity, positionParameter_.velocityRandomRange),
		RandomRangeVector3(positionParameter_.acceleration, positionParameter_.accelerationRandomRange)
	};

	particle.rotationPara = {
		RandomRangeVector3(rotationParameter_.startNum, rotationParameter_.startRandomRange),
		RandomRangeVector3(rotationParameter_.endNum, rotationParameter_.endRandomRange),
		RandomRangeVector3(rotationParameter_.velocity, rotationParameter_.velocityRandomRange),
		RandomRangeVector3(rotationParameter_.acceleration, rotationParameter_.accelerationRandomRange)
	};

	particle.scalePara = {
		RandomRangeVector3(scaleParameter_.startNum, scaleParameter_.startRandomRange),
		RandomRangeVector3(scaleParameter_.endNum, scaleParameter_.endRandomRange),
		RandomRangeVector3(scaleParameter_.velocity, scaleParameter_.velocityRandomRange),
		RandomRangeVector3(scaleParameter_.acceleration, scaleParameter_.accelerationRandomRange)
	};

	particle.colorPara = {
		RandomRangeVector4(colorParameter_.startColor, colorParameter_.startRandomRange),
		RandomRangeVector4(colorParameter_.endColor, colorParameter_.endRandomRange),
		RandomRangeVector4(colorParameter_.velocity, colorParameter_.velocityRandomRange),
		RandomRangeVector4(colorParameter_.acceleration, colorParameter_.accelerationRandomRange)
	};

	particle.transform.Initialize();

	particle.transform.scale_ = particle.scalePara.startNum;
	particle.transform.rotate_ = particle.rotationPara.startNum;
	particle.transform.translate_ = particle.positionPara.startNum;

	particle.color = particle.colorPara.startColor;

	particle.lifeTime = RandomRangeFloat(particleLifeTime_, particleLifeTimeRandomRange_);
	particle.currentTime = 0.0f;

	return particle;
}
#include "ParticleEmitter.h"

#include "Base/DirectXCommon.h"
#include "Base/View/SRVManager.h"
#include "Base/Renderer.h"
#include "Base/Input.h"
#include "2d/Sprite/TextureManager.h"
#include "3d/Camera/Camera.h"
#include "3d/Particle/ParticleCommon.h"
#include "3d/Model/ModelManager.h"
#include "3d/Collider/ColliderManager.h"

#include <Math/Utility/MakeMatrixMath.h>
#include <Math/Utility/Easing.h>
#include <Math/Utility/Random.h>

#include "imgui.h"

#include "json.hpp"
#include "fstream"
#include "numbers"

namespace MyEngine {

	//インスタンスの最大数
	const uint32_t ParticleEmitter::kNumMaxInstance = 1000;

	//1フレームで進む秒数
	const float ParticleEmitter::kDeltaTime = 1.0f / 60.0f;

	///=====================================================/// 
	/// パーティクルエミッターのデストラクタ
	///=====================================================///
	ParticleEmitter::~ParticleEmitter() {

		//解放されるメモリ番号を登録
		srvManager_->RecordFreeIndex(srvIndex_);
	}

	///=====================================================/// 
	/// パーティクルエミッターを初期化
	///=====================================================///
	void ParticleEmitter::Initialize(const std::string& groupName, const std::string& fileName, ParticleCommon* particleCommonPtr, Camera* cameraPtr, Input* inputPtr, Renderer* rendererPtr) {

		/// === シングルトンインスタンスの取得 === ///

		particleCommon_ = particleCommonPtr;

		//DirectX基底のインスタンスを取得
		directXCommon_ = particleCommon_->GetDxCommon();

		//テクスチャマネージャーのインスタンスを取得
		textureManager_ = TextureManager::GetInstance();

		//SRVマネージャーのインスタンスを取得
		srvManager_ = particleCommon_->GetSRVManager();

		//カメラ情報の設定
		defaultCamera_ = cameraPtr;

		input_ = inputPtr;

		renderer_ = rendererPtr;

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

		//ディレクトリパスの設定
		directoryPath_ = "Resource/Json/Particle/Group/";

		//ワールドトランスフォームの初期化
		emitterWorldTransform_.Initialize();

		//パラメータの読み込み
		ImportEmitterData(groupName, fileName);

		//タイマーの初期化
		emitTimer_ = 0.0f;

		//生成数の初期化
		emitCount_ = 0;

		//生成フラグの初期化
		isEmit_ = false;

		//アクティブの初期化
		isActive_ = true;

		/// === パーティクル情報の初期化 === ///

		if (modelFileName_.find(".obj") != std::string::npos) {

			//モデルの読み込み
			ModelManager::GetInstance()->LoadModel(modelName_, modelFileName_);
		}

		//モデルの生成
		model_ = ModelManager::GetInstance()->FindModel(modelName_);

		//テクスチャのロード
		textureManager_->LoadTexture("Resource/Sprite/Particle/" + textureFileName_);

		//テクスチャの設定
		model_->SetTextureFilePath("Resource/Sprite/Particle/" + textureFileName_);

		//テクスチャ番号の設定
		model_->SetTextureIndex(textureManager_->GetSrvIndex(model_->GetTextureFilePath()));
	}

	///=====================================================/// 
	/// パーティクルエミッターの更新処理
	///=====================================================///
	void ParticleEmitter::Update() {

		//ワールドトランスフォームの更新
		emitterWorldTransform_.UpdateMatrix();

		//アクティブフラグがfalseの場合は更新しない
		if (!isActive_) {
			return;
		}

		/// === パーティクルの生成 === ///

		//タイマーの更新
		emitTimer_ += 1.0f / 60.0f;

		//生成フラグがtrueの場合はパーティクルを生成する
		if (isEmit_) {

			//登録するパーティクル
			std::list<Particle> particles;

			//タイマーが生成間隔を超えた場合はパーティクルを生成する
			while (emitTimer_ >= emitFrequency_) {

				//最大生成数以下であれば生成する
				if (emitCount_ >= emitMaxCount_) {

					//生成フラグをfalseにする
					isEmit_ = false;

					//無限生成フラグがtrueの場合は生成を続ける
					if (isInfinity_) {

						Emit();
					}

					break;
				}

				//新しいパーティクルの生成
				particles.push_back(MakeNewParticle());

				//タイマーを生成間隔分減らす
				emitTimer_ -= emitFrequency_;

				//生成数をカウントアップ
				emitCount_++;
			}

			//パーティクルをリストに追加
			particles_.splice(particles_.end(), particles);
		}

		/// === パーティクルの更新 === ///

		//カメラからビュープロジェクション行列を取得
		Matrix4x4 viewProjectionMatrix = defaultCamera_->Get3DViewProjectionMatrix();

		//カメラからビュー行列を取得
		Matrix4x4 viewMatrix = defaultCamera_->GetViewMatrix();

		viewMatrix.m[3][0] = 0.0f;
		viewMatrix.m[3][1] = 0.0f;
		viewMatrix.m[3][2] = 0.0f;
		viewMatrix.m[3][3] = 1.0f;

		//ビルボード用の行列を計算
		Matrix4x4 billboardMatrix = Inverse4x4(viewMatrix);

		//インスタンス数の初期化
		numInstance_ = 0;

		//パーティクルの更新
		for (std::list<Particle>::iterator particle = particles_.begin();
			particle != particles_.end();) {

			//生存時間を超えたパーティクルは削除する
			if (particle->lifeTime <= particle->currentTime) {
				particle = particles_.erase(particle);
				continue;
			}

			if (numInstance_ < kNumMaxInstance) {

				/// === 座標の計算 === ///

				UpdateParameter(
					particle->transform.translate_,
					particle->positionPara,
					positionUpdateState_,
					positionEasingState_,
					positionEasingStrength_,
					particle->currentTime,
					particle->lifeTime
				);

				/// === 回転角の計算 === ///

				UpdateParameter(
					particle->transform.rotate_,
					particle->rotationPara,
					rotationUpdateState_,
					rotationEasingState_,
					rotationEasingStrength_,
					particle->currentTime,
					particle->lifeTime
				);

				/// === 拡縮の計算 === ///

				UpdateParameter(
					particle->transform.scale_,
					particle->scalePara,
					scaleUpdateState_,
					scaleEasingState_,
					scaleEasingStrength_,
					particle->currentTime,
					particle->lifeTime
				);

				/// === 色の計算 === ///

				switch (colorUpdateState_) {
				case ParticleEmitter::UpdateState::START:

					particle->color = particle->colorPara.startColor;

					break;
				case ParticleEmitter::UpdateState::VELOCITY:

					particle->colorPara.velocity = particle->colorPara.velocity + particle->colorPara.acceleration * kDeltaTime;

					particle->color = particle->color + particle->colorPara.velocity * kDeltaTime;

					break;
				case ParticleEmitter::UpdateState::EASING:

					float time = particle->currentTime / particle->lifeTime;

					switch (colorEasingState_) {
					case ParticleEmitter::EasingState::LINEAR:

						particle->color = Lerp(
							particle->colorPara.startColor,
							particle->colorPara.endColor,
							time
						);

						break;
					case ParticleEmitter::EasingState::EASE_IN:

						particle->color = EaseIn(
							particle->colorPara.startColor,
							particle->colorPara.endColor,
							time,
							colorEasingStrength_
						);

						break;
					case ParticleEmitter::EasingState::EASE_OUT:

						particle->color = EaseOut(
							particle->colorPara.startColor,
							particle->colorPara.endColor,
							time,
							colorEasingStrength_
						);

						break;
					case ParticleEmitter::EasingState::EASE_INOUT:

						particle->color = EaseInOut(
							particle->colorPara.startColor,
							particle->colorPara.endColor,
							time,
							colorEasingStrength_
						);

						break;
					}
					break;
				}

				if (!isLoop_) {

					//ループしない場合は時間を進める
					particle->currentTime += kDeltaTime;
				}

				//地面以下に行かないようにする
				if (particle->transform.translate_.y <= 0.0f) {

					particle->transform.translate_.y = 0.0f;

					particle->positionPara.velocity.x = 0.0f;
					particle->positionPara.velocity.z = -80.0f;
				}

				//パーティクルのワールド行列を更新
				particle->transform.UpdateMatrix();

				//NOTE:エミッターの座標との親子関係ができていない。改良の必要あり。

				////エミッターのワールド行列を取得
				//Matrix4x4 translateEMatrix = emitterWorldTransform_.GetLocalTranslateMatrix();
				//Matrix4x4 rotateEMatrix = emitterWorldTransform_.GetLocalRotateMatrix();
				//Matrix4x4 scaleEMatrix = emitterWorldTransform_.GetLocalScaleMatrix();

				//translateMatrix = translateMatrix/* * translateEMatrix*/;
				//rotateMatrix = rotateMatrix/* * rotateEMatrix*/;
				//scaleMatrix = scaleMatrix/* * scaleEMatrix*/;

				//パーティクルのワールド行列を取得
				Matrix4x4 translateMatrix = particle->transform.GetLocalTranslateMatrix();
				Matrix4x4 rotateMatrix = particle->transform.GetLocalRotateMatrix();
				Matrix4x4 scaleMatrix = particle->transform.GetLocalScaleMatrix();

				//ビルボードの場合はZ軸の回転を行う
				if (isBillboard_) {

					//Z軸の回転行列を作成
					Matrix4x4 zRot = MakeRotateZMatrix(particle->transform.rotate_.z);

					//ビルボード行列を掛け合わせる
					rotateMatrix = zRot * billboardMatrix;
				}

				//ワールド行列の計算
				Matrix4x4 worldMatrix = (scaleMatrix * rotateMatrix) * translateMatrix;

				//ワールドビュープロジェクション行列の合成
				Matrix4x4 worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;

				//更新後のワールド行列を設定
				particle->transform.SetWorldMatrix(worldMatrix);

				//インスタンシングデータに書き込む
				instancingData_[numInstance_].WVP = worldViewProjectionMatrix;
				instancingData_[numInstance_].World = worldMatrix;
				instancingData_[numInstance_].color = particle->color;
				instancingData_[numInstance_].color.w = particle->color.w;

				//インスタンス数をカウントアップ
				numInstance_++;
			}

			//次のパーティクルへ
			particle++;
		}
	}

	///=====================================================/// 
	/// パーティクルエミッターの描画処理
	///=====================================================///
	void ParticleEmitter::Draw(LayerType layer) {

		//Renderクラスに渡す
		std::function<void()> command;

		if (!isActive_) {
			return;
		}

		if (numInstance_ <= 0) {
			return;
		}

		command = [this]() {

			//パーティクルの描画前処理
			particleCommon_->CommonDrawSetting();

			// インスタンシング用データ(SRV)の設定
			directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(srvIndex_));

			// モデルの各パーツを描画
			for (size_t i = 0; i < model_->GetMeshParts().size(); i++) {
				auto mesh = model_->GetMesh(i);

				// メッシュデータの設定
				mesh->SendDataForGPU();

				// マテリアルデータの設定
				directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, model_->GetMeshParts()[i].materialResource.Get()->GetGPUVirtualAddress());

				// テクスチャデータの設定
				directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(model_->GetTextureFilePath(i)));

				// インスタンシング描画コマンド発行
				directXCommon_->GetCommandList()->DrawIndexedInstanced(mesh->GetIndexCount(), numInstance_, 0, 0, 0);
			}
			};

		//レンダラーにコマンドを登録
		renderer_->AddDraw(layer, true, command);
	}

	///=====================================================/// 
	/// ImGuiの表示
	///=====================================================///
	void ParticleEmitter::ImGui() {

#ifdef _USE_IMGUI

		std::string currentName = name_;

		if (ImGui::BeginTabItem(name_.c_str())) {

			if (ImGui::CollapsingHeader("エミッター設定")) {
				ImGui::Columns(2, "EmitterColumns", false);

				ImGui::Text("名前");
				if (ImGui::InputText("##Name", currentName.data(), 256)) {
					if (input_->IsTriggerPushKey(DIK_RETURN)) {
						name_ = currentName.c_str();
					}
				}
				ImGui::NextColumn();
				ImGui::NextColumn();

				std::vector<const char*> modelItems;

				int currentModel = 0;

				modelItems.push_back("モデルを選択");
				modelItems.push_back("Plane");
				modelItems.push_back("Ring");
				modelItems.push_back("Cylinder");
				modelItems.push_back("Sphere");

				ImGui::Text("モデル");
				if (ImGui::Combo("##Model", &currentModel, modelItems.data(), static_cast<int>(modelItems.size()))) {

					modelName_ = modelItems[currentModel];

					modelFileName_ = "";

					if (modelFileName_.find(".obj")) {

						//モデルの読み込み
						ModelManager::GetInstance()->LoadModel(modelName_, modelFileName_);
					}

					model_.reset();

					model_ = ModelManager::GetInstance()->FindModel(modelName_);
				}
				ImGui::NextColumn();

				std::vector<const char*> textureItems;

				int currentTexture = 0;

				for (auto& textureName : textureList_) {

					textureItems.push_back(textureName.c_str());
				}

				textureItems.insert(textureItems.begin(), "テクスチャを選択");

				ImGui::Text("テクスチャ");
				if (ImGui::Combo("##Texture", &currentTexture, textureItems.data(), static_cast<int>(textureItems.size()))) {

					textureFileName_ = textureItems[currentTexture];

					model_->SetTextureFilePath("Resource/Sprite/Particle/" + textureFileName_);

					model_->SetTextureIndex(textureManager_->GetSrvIndex(model_->GetTextureFilePath()));
				}
				ImGui::NextColumn();

				ImGui::Separator();

				ImGui::Text("エミッター座標");
				ImGui::DragFloat3("##EmitterPosition", &emitterWorldTransform_.translate_.x, 0.1f);
				ImGui::NextColumn();

				ImGui::Text("エミッター角度");
				ImGui::DragFloat3("##EmitterRotation", &emitterWorldTransform_.rotate_.x, 0.01f);
				ImGui::NextColumn();

				ImGui::Text("エミッター拡縮");
				ImGui::DragFloat3("##EmitterScale", &emitterWorldTransform_.scale_.x, 0.1f);
				ImGui::NextColumn();
				ImGui::NextColumn();

				ImGui::Separator();

				ImGui::Text("生成数");
				ImGui::InputInt("##EmitCount", &emitMaxCount_);
				ImGui::NextColumn();

				ImGui::Text("生成間隔");
				ImGui::DragFloat("##frequency", &emitFrequency_, 0.01f);
				ImGui::NextColumn();

				ImGui::Text("生存時間");
				ImGui::DragFloat("##lifeTime", &particleLifeTime_, 0.1f);
				ImGui::NextColumn();

				ImGui::Text("生存時間のランダム幅");
				ImGui::DragFloat("##lifeTimeRandomRange", &particleLifeTimeRandomRange_, 0.1f);
				ImGui::NextColumn();

				ImGui::Text("ループ");
				ImGui::Checkbox("##isLoop", &isLoop_);
				ImGui::NextColumn();

				ImGui::Text("生成数無限");
				ImGui::Checkbox("##isInfinity", &isInfinity_);
				ImGui::NextColumn();

				ImGui::Text("ビルボード");
				ImGui::Checkbox("##isBillboard", &isBillboard_);
				ImGui::NextColumn();

				ImGui::Text("アクティブ");
				ImGui::Checkbox("##isActive", &isActive_);
				ImGui::NextColumn();

				ImGui::Columns(1);
			}

			if (ImGui::CollapsingHeader("生成座標")) {
				ImGui::Columns(2, "PositionColumns", false);

				ImGuiParameter(
					"Position",
					positionParameter_,
					positionUpdateState_,
					positionEasingState_,
					positionEasingStrength_
				);

				ImGui::Columns(1);
			}

			if (ImGui::CollapsingHeader("生成角度")) {
				ImGui::Columns(2, "RotationColumns", false);

				ImGuiParameter(
					"Rotation",
					rotationParameter_,
					rotationUpdateState_,
					rotationEasingState_,
					rotationEasingStrength_
				);

				ImGui::Columns(1);
			}

			if (ImGui::CollapsingHeader("生成拡縮")) {
				ImGui::Columns(2, "ScaleColumns", false);

				ImGuiParameter(
					"Scale",
					scaleParameter_,
					scaleUpdateState_,
					scaleEasingState_,
					scaleEasingStrength_
				);

				ImGui::Columns(1);
			}

			if (ImGui::CollapsingHeader("生成色")) {
				ImGui::Columns(2, "ColorColumns", false);

				const char* stateItems[] = { "StartAnimation","Velocity","Easing" };

				int currentState = static_cast<int>(colorUpdateState_);

				ImGui::Text("更新ステート");
				if (ImGui::Combo(CreateLabelName("Color", "UpdateState").c_str(), &currentState, stateItems, IM_ARRAYSIZE(stateItems))) {

					colorUpdateState_ = static_cast<UpdateState>(currentState);
				}
				ImGui::NextColumn();
				ImGui::NextColumn();

				switch (colorUpdateState_) {
				case ParticleEmitter::UpdateState::START:

					ImGui::Text("初期値");
					ImGui::ColorEdit4(CreateLabelName("Color", "StartNum").c_str(), &colorParameter_.startColor.x);
					ImGui::NextColumn();

					ImGui::Text("初期値のランダム幅");
					ImGui::ColorEdit4(CreateLabelName("Color", "StartRandomRange").c_str(), &colorParameter_.startRandomRange.x);
					ImGui::NextColumn();

					break;
				case ParticleEmitter::UpdateState::VELOCITY:

					ImGui::Text("初期値");
					ImGui::ColorEdit4(CreateLabelName("Color", "StartNum").c_str(), &colorParameter_.startColor.x);
					ImGui::NextColumn();

					ImGui::Text("初期値のランダム幅");
					ImGui::ColorEdit4(CreateLabelName("Color", "StartRandomRange").c_str(), &colorParameter_.startRandomRange.x);
					ImGui::NextColumn();

					ImGui::Text("終了値");
					ImGui::ColorEdit4(CreateLabelName("Color", "EndNum").c_str(), &colorParameter_.endColor.x);
					ImGui::NextColumn();

					ImGui::Text("終了値のランダム幅");
					ImGui::ColorEdit4(CreateLabelName("Color", "EndRandomRange").c_str(), &colorParameter_.endRandomRange.x);
					ImGui::NextColumn();

					ImGui::Text("移動量");
					ImGui::ColorEdit4(CreateLabelName("Color", "Velocity").c_str(), &colorParameter_.velocity.x);
					ImGui::NextColumn();

					ImGui::Text("移動量のランダム幅");
					ImGui::ColorEdit4(CreateLabelName("Color", "VelocityRandomRange").c_str(), &colorParameter_.velocityRandomRange.x);
					ImGui::NextColumn();

					ImGui::Text("加速度");
					ImGui::ColorEdit4(CreateLabelName("Color", "Acceleration").c_str(), &colorParameter_.acceleration.x);
					ImGui::NextColumn();

					ImGui::Text("加速度のランダム幅");
					ImGui::ColorEdit4(CreateLabelName("Color", "AccelerationRandomRange").c_str(), &colorParameter_.accelerationRandomRange.x);
					ImGui::NextColumn();

					break;
				case ParticleEmitter::UpdateState::EASING:

					const char* easingStateItems[] = { "Lerp","EaseIn","EaseOut","EaseInOut" };

					int currentEasingState = static_cast<int>(colorEasingState_);

					ImGui::Text("イージングステート");
					if (ImGui::Combo(CreateLabelName("Color", "EasingState").c_str(), &currentEasingState, easingStateItems, IM_ARRAYSIZE(easingStateItems))) {

						colorEasingState_ = static_cast<EasingState>(currentEasingState);
					}
					ImGui::NextColumn();

					ImGui::Text("イージング強度");
					ImGui::InputFloat(CreateLabelName("Color", "EasingStrength").c_str(), &colorEasingStrength_);
					ImGui::NextColumn();

					ImGui::Text("初期値");
					ImGui::ColorEdit4(CreateLabelName("Color", "StartNum").c_str(), &colorParameter_.startColor.x);
					ImGui::NextColumn();

					ImGui::Text("初期値のランダム幅");
					ImGui::ColorEdit4(CreateLabelName("Color", "StartRandomRange").c_str(), &colorParameter_.startRandomRange.x);
					ImGui::NextColumn();

					ImGui::Text("終了値");
					ImGui::ColorEdit4(CreateLabelName("Color", "EndNum").c_str(), &colorParameter_.endColor.x);
					ImGui::NextColumn();

					ImGui::Text("終了値のランダム幅");
					ImGui::ColorEdit4(CreateLabelName("Color", "EndRandomRange").c_str(), &colorParameter_.endRandomRange.x);
					ImGui::NextColumn();

					break;
				}

				ImGui::Columns(1);
			}

			if (ImGui::Button("生成")) {
				Emit();
			}

			ImGui::EndTabItem();
		}

#endif // _USE_IMGUI

	}

	///=====================================================/// 
	/// パーティクルの生成を開始
	///=====================================================///
	void ParticleEmitter::Emit() {

		isEmit_ = true;

		emitCount_ = 0;

		emitTimer_ = 0;
	}

	///=====================================================/// 
	/// エミッターの設定情報をJSONファイルに書き出し
	///=====================================================///
	void ParticleEmitter::ExportEmitterData(const std::string& groupName) {

		nlohmann::json jsonData;

		std::string directoryPath = directoryPath_ + groupName + "/Emitter/";

		std::string filePath = directoryPath + name_ + ".json";

		jsonData["name"] = name_;

		jsonData["modelName"] = modelName_;

		jsonData["modelFileName"] = modelFileName_;

		jsonData["textureFileName"] = textureFileName_;

		jsonData["lifeTime"] = particleLifeTime_;
		jsonData["lifeTimeRandomRange"] = particleLifeTimeRandomRange_;
		jsonData["frequency"] = emitFrequency_;
		jsonData["maxCount"] = emitMaxCount_;

		jsonData["isLoop"] = isLoop_;
		jsonData["isInfinity"] = isInfinity_;
		jsonData["isBillboard"] = isBillboard_;

		jsonData["position"] = {
			{"updateState",{positionUpdateState_}},
			{"easingState",{positionEasingState_}},
			{"easingStrength",{positionEasingStrength_}},
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
			{"updateState",{rotationUpdateState_}},
			{"easingState",{rotationEasingState_}},
			{"easingStrength",{rotationEasingStrength_}},
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
			{"updateState",{scaleUpdateState_}},
			{"easingState",{scaleEasingState_}},
			{"easingStrength",{scaleEasingStrength_}},
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
			{"updateState",{colorUpdateState_}},
			{"easingState",{colorEasingState_}},
			{"easingStrength",{colorEasingStrength_}},
			{"startColor", {colorParameter_.startColor.x,colorParameter_.startColor.y,colorParameter_.startColor.z,colorParameter_.startColor.w}},
			{"startRandomRange", {colorParameter_.startRandomRange.x,colorParameter_.startRandomRange.y,colorParameter_.startRandomRange.z,colorParameter_.startRandomRange.w}},
			{"endColor", {colorParameter_.endColor.x,colorParameter_.endColor.y,colorParameter_.endColor.z,colorParameter_.endColor.w}},
			{"endRandomRange", {colorParameter_.endRandomRange.x,colorParameter_.endRandomRange.y,colorParameter_.endRandomRange.z,colorParameter_.endRandomRange.w}},
			{"velocity", {colorParameter_.velocity.x,colorParameter_.velocity.y,colorParameter_.velocity.z,colorParameter_.velocity.w}},
			{"velocityRandomRange", {colorParameter_.velocityRandomRange.x,colorParameter_.velocityRandomRange.y,colorParameter_.velocityRandomRange.z,colorParameter_.velocityRandomRange.w}},
			{"acceleration", {colorParameter_.acceleration.x,colorParameter_.acceleration.y,colorParameter_.acceleration.z,colorParameter_.acceleration.w}},
			{"accelerationRandomRange", {colorParameter_.accelerationRandomRange.x,colorParameter_.accelerationRandomRange.y,colorParameter_.accelerationRandomRange.z,colorParameter_.accelerationRandomRange.w}}
		};

		std::filesystem::path dir(directoryPath);
		if (!std::filesystem::exists(directoryPath)) {
			std::filesystem::create_directory(directoryPath);
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

	///=====================================================/// 
	/// JSONファイルからエミッターの設定情報を読み込み
	///=====================================================///
	void ParticleEmitter::ImportEmitterData(const std::string& groupName, const std::string& fileName) {

		nlohmann::json jsonData;

		std::string directoryPath = directoryPath_ + groupName + "/Emitter/";

		std::string filePath = directoryPath + fileName + ".json";

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

		modelName_ = jsonData["modelName"];

		modelFileName_ = jsonData["modelFileName"];

		textureFileName_ = jsonData["textureFileName"];

		particleLifeTime_ = jsonData["lifeTime"];
		particleLifeTimeRandomRange_ = jsonData["lifeTimeRandomRange"];
		emitFrequency_ = jsonData["frequency"];
		emitMaxCount_ = jsonData["maxCount"];

		isLoop_ = jsonData["isLoop"];
		isInfinity_ = jsonData["isInfinity"];
		isBillboard_ = jsonData["isBillboard"];

		if (jsonData.contains("position")) {

			auto position = jsonData["position"];

			positionUpdateState_ = position["updateState"][0];
			positionEasingState_ = position["easingState"][0];
			positionEasingStrength_ = position["easingStrength"][0];

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

			rotationUpdateState_ = rotation["updateState"][0];
			rotationEasingState_ = rotation["easingState"][0];
			rotationEasingStrength_ = rotation["easingStrength"][0];

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

			scaleUpdateState_ = scale["updateState"][0];
			scaleEasingState_ = scale["easingState"][0];
			scaleEasingStrength_ = scale["easingStrength"][0];

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

			colorUpdateState_ = color["updateState"][0];
			colorEasingState_ = color["easingState"][0];
			colorEasingStrength_ = color["easingStrength"][0];

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

	///=====================================================/// 
	/// 新しいパーティクルを生成し、初期パラメータを設定
	///=====================================================///
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

		particle.positionPara.startNum += emitterWorldTransform_.GetWorldTranslate();

		particle.positionPara.endNum += emitterWorldTransform_.GetWorldTranslate();

		particle.transform.Initialize();

		particle.transform.scale_ = particle.scalePara.startNum;
		particle.transform.rotate_ = particle.rotationPara.startNum;
		particle.transform.translate_ = particle.positionPara.startNum;

		particle.color = particle.colorPara.startColor;

		particle.lifeTime = RandomRangeFloat(particleLifeTime_, particleLifeTimeRandomRange_);
		particle.currentTime = 0.0f;

		return particle;
	}

	///=====================================================/// 
	/// パーティクルの各パラメータ（位置・回転・スケールなど）を更新
	///=====================================================///
	void ParticleEmitter::UpdateParameter(Vector3& num, ParticleParameter& parameter, UpdateState& updateState, EasingState& easingState, float& easingStrength, float& currentTime, float& lifeTime) {

		switch (updateState) {
		case ParticleEmitter::UpdateState::START:

			num = parameter.startNum;

			break;
		case ParticleEmitter::UpdateState::VELOCITY:

			parameter.velocity = parameter.velocity + parameter.acceleration * kDeltaTime;

			num = num + parameter.velocity * kDeltaTime;

			break;
		case ParticleEmitter::UpdateState::EASING:

			float time = currentTime / lifeTime;

			switch (easingState) {
			case ParticleEmitter::EasingState::LINEAR:

				num = Lerp(
					parameter.startNum,
					parameter.endNum,
					time
				);

				break;
			case ParticleEmitter::EasingState::EASE_IN:

				num = EaseIn(
					parameter.startNum,
					parameter.endNum,
					time,
					easingStrength
				);

				break;
			case ParticleEmitter::EasingState::EASE_OUT:

				num = EaseOut(
					parameter.startNum,
					parameter.endNum,
					time,
					easingStrength
				);

				break;
			case ParticleEmitter::EasingState::EASE_INOUT:

				num = EaseInOut(
					parameter.startNum,
					parameter.endNum,
					time,
					easingStrength
				);

				break;
			}
			break;
		}
	}

	///=====================================================/// 
	/// パラメータのImGui表示
	///=====================================================///
	void ParticleEmitter::ImGuiParameter(std::string labelName, EmitterParameter& parameter, UpdateState& updateState, EasingState& easingState, float& easingStrength) {

#ifdef _USE_IMGUI

		/// === 更新ステート === ///

		const char* stateItems[] = { "StartAnimation","Velocity","Easing" };

		int currentState = static_cast<int>(updateState);

		ImGui::Text("更新ステート");
		if (ImGui::Combo(CreateLabelName(labelName, "UpdateState").c_str(), &currentState, stateItems, IM_ARRAYSIZE(stateItems))) {

			updateState = static_cast<UpdateState>(currentState);
		}
		ImGui::NextColumn();
		ImGui::NextColumn();

		switch (updateState) {
		case ParticleEmitter::UpdateState::START:

			ImGui::Text("初期値");
			ImGui::DragFloat3(CreateLabelName(labelName, "StartNum").c_str(), &parameter.startNum.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("初期値のランダム幅");
			ImGui::DragFloat3(CreateLabelName(labelName, "StartRandomRange").c_str(), &parameter.startRandomRange.x, 0.1f);
			ImGui::NextColumn();

			break;
		case ParticleEmitter::UpdateState::VELOCITY:

			ImGui::Text("初期値");
			ImGui::DragFloat3(CreateLabelName(labelName, "StartNum").c_str(), &parameter.startNum.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("初期値のランダム幅");
			ImGui::DragFloat3(CreateLabelName(labelName, "StartRandomRange").c_str(), &parameter.startRandomRange.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("終了値");
			ImGui::DragFloat3(CreateLabelName(labelName, "EndNum").c_str(), &parameter.endNum.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("終了値のランダム幅");
			ImGui::DragFloat3(CreateLabelName(labelName, "EndRandomRange").c_str(), &parameter.endRandomRange.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("移動量");
			ImGui::DragFloat3(CreateLabelName(labelName, "Velocity").c_str(), &parameter.velocity.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("移動量のランダム幅");
			ImGui::DragFloat3(CreateLabelName(labelName, "VelocityRandomRange").c_str(), &parameter.velocityRandomRange.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("加速度");
			ImGui::DragFloat3(CreateLabelName(labelName, "Acceleration").c_str(), &parameter.acceleration.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("加速度のランダム幅");
			ImGui::DragFloat3(CreateLabelName(labelName, "AccelerationRandomRange").c_str(), &parameter.accelerationRandomRange.x, 0.1f);
			ImGui::NextColumn();

			break;
		case ParticleEmitter::UpdateState::EASING:

			const char* easingStateItems[] = { "Lerp","EaseIn","EaseOut","EaseInOut" };

			int currentEasingState = static_cast<int>(easingState);

			ImGui::Text("イージングステート");
			if (ImGui::Combo(CreateLabelName(labelName, "EasingState").c_str(), &currentEasingState, easingStateItems, IM_ARRAYSIZE(easingStateItems))) {

				easingState = static_cast<EasingState>(currentEasingState);
			}
			ImGui::NextColumn();

			ImGui::Text("イージング強度");
			ImGui::InputFloat(CreateLabelName(labelName, "EasingStrength").c_str(), &easingStrength);
			ImGui::NextColumn();

			ImGui::Text("初期値");
			ImGui::DragFloat3(CreateLabelName(labelName, "StartNum").c_str(), &parameter.startNum.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("初期値のランダム幅");
			ImGui::DragFloat3(CreateLabelName(labelName, "StartRandomRange").c_str(), &parameter.startRandomRange.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("終了値");
			ImGui::DragFloat3(CreateLabelName(labelName, "EndNum").c_str(), &parameter.endNum.x, 0.1f);
			ImGui::NextColumn();

			ImGui::Text("終了値のランダム幅");
			ImGui::DragFloat3(CreateLabelName(labelName, "EndRandomRange").c_str(), &parameter.endRandomRange.x, 0.1f);
			ImGui::NextColumn();

			break;
		}

#endif // _USE_IMGUI

	}

	///=====================================================/// 
	/// ImGuiなどで使用するユニークラベル名を生成
	///=====================================================///
	std::string ParticleEmitter::CreateLabelName(std::string labelName, const char* label) {

		return "##" + labelName + label;
	}
}
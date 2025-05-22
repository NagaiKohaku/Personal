#include "EmitterGroup.h"

#include "Base/Input.h"

#include "json.hpp"
#include "filesystem"
#include "fstream"

#include "imgui.h"

void EmitterGroup::Initialize(Camera* ptr) {

	camera_ = ptr;

	directoryPath = "Resource/Json/Particle/Group/";

	//エミッターの初期化
	particleEmitters_.clear();

	transform_.Initialize();

}

void EmitterGroup::Update() {

	transform_.UpdateMatrix();

	//すべてのパーティクルグループの処理をする
	for (auto& emitter : particleEmitters_) {

		emitter->SetWorldTransform(transform_);

		emitter->Update();
	}
}

void EmitterGroup::Draw() {
	//すべてのパーティクルグループの処理をする
	for (auto& emitter : particleEmitters_) {
		emitter->Draw(Object);
	}
}

void EmitterGroup::ImGui() {

	std::string currentName = name_;

	if (ImGui::BeginTabBar("EmitterGroup")) {

		if (ImGui::BeginTabItem(name_.c_str())) {

			ImGui::Text("Name");
			if (ImGui::InputText("##Name", currentName.data(), 256)) {
				if (Input::GetInstance()->IsTriggerPushKey(DIK_RETURN)) {
					name_ = currentName.c_str();
				}
			}

			if (ImGui::Button("Save")) {
				SaveEmitter();
			}

			if (ImGui::Button("AddEmitter")) {
				AddEmitter();
			}

			if (ImGui::BeginTabBar(name_.c_str())) {

				for (auto& emitter : particleEmitters_) {

					emitter->ImGui();
				}

				ImGui::EndTabBar();
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

}

void EmitterGroup::LoadEmitter(std::string fileName) {

	nlohmann::json jsonData;

	std::string filePath = directoryPath + fileName + ".json";

	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::string message = "Failed open data file for read.";
		MessageBoxA(nullptr, message.c_str(), "EmitterGroup", 0);
		assert(0);
		return;
	}

	file >> jsonData;

	file.close();

	for (auto& data : jsonData) {

		if (data.contains("name")) {

			name_ = data["name"];

		} else if (data.contains("emitter")) {

			std::string fileName = data["emitter"];

			std::unique_ptr<ParticleEmitter> newEmitter;

			newEmitter = std::make_unique<ParticleEmitter>();

			newEmitter->SetTextureList(textureList_);

			newEmitter->Initialize(fileName, camera_);

			particleEmitters_.push_back(std::move(newEmitter));
		}
	}
}

void EmitterGroup::SaveEmitter() {

	nlohmann::json jsonData = nlohmann::json::array();

	std::string filePath = directoryPath + name_ + ".json";

	nlohmann::json nameData;

	nameData["name"] = name_;

	jsonData.push_back(nameData);

	for (auto& emitter : particleEmitters_) {
		nlohmann::json entry;
		entry["emitter"] = emitter->GetName();
		jsonData.push_back(entry);
	}

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

	for (auto& emitter : particleEmitters_) {

		emitter->ExportEmitterData();
	}
}

void EmitterGroup::AddEmitter() {

	for (auto& emitter : particleEmitters_) {

		if (emitter->GetName() == "default") {

			return;
		}
	}

	std::unique_ptr<ParticleEmitter> newEmitter;

	newEmitter = std::make_unique<ParticleEmitter>();

	newEmitter->SetTextureList(textureList_);

	newEmitter->Initialize("default", camera_);

	particleEmitters_.push_back(std::move(newEmitter));
}

void EmitterGroup::Emit() {

	for (auto& emitter : particleEmitters_) {
		emitter->Emit();
	}
}

std::list<ParticleEmitter*> EmitterGroup::GetEmitterList() {

	std::list<ParticleEmitter*> list;

	for (auto& emitter : particleEmitters_) {

		list.push_back(emitter.get());
	}

	return list;
}

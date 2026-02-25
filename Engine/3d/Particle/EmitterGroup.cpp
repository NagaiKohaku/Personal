#include "EmitterGroup.h"

#include "Base/Input.h"

#include "json.hpp"
#include "filesystem"
#include "fstream"

#include "imgui.h"

namespace MyEngine {

	///=====================================================/// 
	/// パーティクルエミッターグループの初期化
	///=====================================================///
	void EmitterGroup::Initialize(ParticleCommon* particleCommonPtr, Camera* ptr) {

		particleCommon_ = particleCommonPtr;

		//カメラのポインタを設定
		camera_ = ptr;

		//ディレクトリパスを設定
		directoryPath_ = "Resource/Json/Particle/Group/";

		//エミッターの初期化
		particleEmitters_.clear();

		//トランスフォームの初期化
		transform_.Initialize();
	}

	///=====================================================/// 
	/// パーティクルエミッターグループの更新処理
	///=====================================================///
	void EmitterGroup::Update() {

		//トランスフォームの更新
		transform_.UpdateMatrix();

		//すべてのパーティクルグループの処理をする
		for (auto& emitter : particleEmitters_) {

			//トランスフォームの設定
			emitter->SetWorldTransform(transform_);

			//エミッターの更新
			emitter->Update();
		}
	}

	///=====================================================/// 
	/// パーティクルエミッターグループの描画
	///=====================================================///
	void EmitterGroup::Draw() {

		//すべてのパーティクルグループの処理をする
		for (auto& emitter : particleEmitters_) {

			//エミッターの描画
			emitter->Draw(LayerType::PARTICLE);
		}
	}

	///=====================================================/// 
	/// ImGuiの表示
	///=====================================================///
	void EmitterGroup::ImGui() {

#ifdef _USE_IMGUI

		std::string currentName = name_;

		if (ImGui::BeginMenuBar()) {

			if (ImGui::BeginMenu("メニュー", "MENU")) {

				if (ImGui::BeginMenu(name_.c_str(), name_.c_str())) {

					if (ImGui::MenuItem("グループの保存")) {

						SaveEmitter();
					}

					if (ImGui::MenuItem("エミッターの追加")) {

						AddEmitter();
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (ImGui::BeginTabBar("EmitterGroup")) {

			if (Input::GetInstance()->IsTriggerPushKey(DIK_SPACE)) {

				Emit();
			}

			if (ImGui::BeginTabItem(name_.c_str())) {

				ImGui::Text("名前");
				if (ImGui::InputText("##Name", currentName.data(), 256)) {
					if (Input::GetInstance()->IsTriggerPushKey(DIK_RETURN)) {
						name_ = currentName.c_str();
					}
				}

				if (ImGui::BeginTabBar(name_.c_str())) {

					for (auto& emitter : particleEmitters_) {

						emitter->ImGui();
					}

					if (ImGui::Button("グループ生成")) {

						Emit();
					}
					ImGui::EndTabBar();
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

#endif // _USE_IMGUI

	}

	///=================================================================/// 
	/// 指定されたJSONファイルからパーティクルエミッター情報を読み込み、グループに登録
	///=================================================================///
	void EmitterGroup::LoadEmitter(std::string fileName) {

		nlohmann::json jsonData;

		std::string directoryPath = directoryPath_ + fileName + "/";

		std::string filePath = directoryPath + fileName + ".json";

		/// === エミッター情報の読み込み === ///

		std::ifstream file(filePath);

		if (!file.is_open()) {
			std::string message = "Failed open data file for read.";
			MessageBoxA(nullptr, message.c_str(), "EmitterGroup", 0);
			assert(0);
			return;
		}

		file >> jsonData;

		file.close();

		/// === エミッター情報の取得 === ///

		for (auto& data : jsonData) {

			if (data.contains("name")) {

				//グループ名の設定
				name_ = data["name"];

			} else if (data.contains("emitter")) {

				//エミッター名の取得
				std::string fileName = data["emitter"];

				//登録するエミッター
				std::unique_ptr<ParticleEmitter> newEmitter;

				//エミッターの生成
				newEmitter = std::make_unique<ParticleEmitter>();

				//テクスチャリストの設定
				newEmitter->SetTextureList(textureList_);

				//エミッターの初期化
				newEmitter->Initialize(name_, fileName, particleCommon_, camera_);

				//リストに登録
				particleEmitters_.push_back(std::move(newEmitter));
			}
		}
	}

	///=====================================================/// 
	/// 現在のパーティクルエミッターグループの情報をJSONファイルに保存
	///=====================================================///
	void EmitterGroup::SaveEmitter() {

		nlohmann::json jsonData = nlohmann::json::array();

		std::string directoryPath = directoryPath_ + name_ + "/";

		std::string filePath = directoryPath + name_ + ".json";

		nlohmann::json nameData;

		//グループ名の保存
		nameData["name"] = name_;

		jsonData.push_back(nameData);

		//リストに登録しているエミッターを走査
		for (auto& emitter : particleEmitters_) {

			nlohmann::json entry;

			//エミッター名を保存
			entry["emitter"] = emitter->GetName();

			jsonData.push_back(entry);
		}


		std::filesystem::path dir(directoryPath);

		//ディレクトリが存在しない場合は作成
		if (!std::filesystem::exists(directoryPath)) {

			//ディレクトリを作成
			std::filesystem::create_directory(directoryPath);
		}

		std::ofstream file;

		file.open(filePath);

		//ファイルが開けなかった場合はエラーメッセージを表示
		if (file.fail()) {

			//エラーメッセージ
			std::string message = "Failed open data file for write.";

			//メッセージボックスを表示
			MessageBoxA(nullptr, message.c_str(), "ParticleEmitter", 0);

			//処理を中断
			assert(0);

			return;
		}

		//データをファイルに書き込む
		file << jsonData.dump(4);

		//ファイルを閉じる
		file.close();

		//すべてのエミッターのパラメータをファイル出力
		for (auto& emitter : particleEmitters_) {

			//エミッターのデータをエクスポート
			emitter->ExportEmitterData(name_);
		}
	}

	///=====================================================/// 
	/// デフォルトのパーティクルエミッターをグループに追加
	///=====================================================///
	void EmitterGroup::AddEmitter() {

		//すでにデフォルトのエミッターが存在する場合は追加しない
		for (auto& emitter : particleEmitters_) {

			//"default"というエミッターがある場合は追加しない
			if (emitter->GetName() == "default") {

				return;
			}
		}

		//登録するためのエミッター
		std::unique_ptr<ParticleEmitter> newEmitter;

		//エミッターの生成
		newEmitter = std::make_unique<ParticleEmitter>();

		//テクスチャリストの設定
		newEmitter->SetTextureList(textureList_);

		//エミッターの初期化
		newEmitter->Initialize("defaultGroup", "default", particleCommon_, camera_);

		//リストに登録
		particleEmitters_.push_back(std::move(newEmitter));
	}

	///=====================================================/// 
	/// グループ内のすべてのパーティクルエミッターからパーティクルを発生
	///=====================================================///
	void EmitterGroup::Emit() {

		//すべてのエミッターに対してパーティクルを発生させる
		for (auto& emitter : particleEmitters_) {

			//パーティクルを発生させる
			emitter->Emit();
		}
	}

	///=====================================================/// 
	/// グループ内のすべてのパーティクルエミッターの発生を停止
	///=====================================================///
	void EmitterGroup::Stop() {

		//すべてのエミッターに対して設定する
		for (auto& emitter : particleEmitters_) {

			//発生を停止する
			emitter->SetIsEmit(false);
		}
	}

	///=====================================================/// 
	/// エミッターリストを取得
	///=====================================================///
	std::list<ParticleEmitter*> EmitterGroup::GetEmitterList() {

		//返すためのリスト
		std::list<ParticleEmitter*> list;

		//すべてのエミッターをリストに追加
		for (auto& emitter : particleEmitters_) {

			//エミッターのポインタをリストに追加
			list.push_back(emitter.get());
		}

		//リストを返す
		return list;
	}
}
#include "Object3D.h"

#include "Base/WinApp.h"
#include "Base/DirectXCommon.h"
#include "Base/Renderer.h"
#include "3d/Object/Object3DCommon.h"
#include "3d/Object/DebugLine.h"
#include "3d/Model/Model.h"
#include "3d/Model/ModelManager.h"
#include "3d/Camera/Camera.h"

#include "Math/MakeMatrixMath.h"

#include "imgui.h"

#include "cassert"

///=====================================================/// 
/// コンストラクタ
///=====================================================///
Object3D::Object3D() {

	//3Dオブジェクト基底のインスタンスを取得
	object3DCommon_ = Object3DCommon::GetInstance();

	//モデルの設定
	model_ = ModelManager::GetInstance()->FindModel("Default");

	//座標変換行列リソースを作成
	WVPResource_ = object3DCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	//書き込むためのアドレスを取得する
	WVPResource_->Map(0, nullptr, reinterpret_cast<void**>(&WVPData_));

	//座標変換行列データの設定
	WVPData_->WVP = MakeIdentity4x4();
	WVPData_->World = MakeIdentity4x4();
	WVPData_->WorldInverseTranspose = MakeIdentity4x4();

	transform_.Initialize();

	//今持っているカメラをデフォルトカメラに設定
	camera_ = object3DCommon_->GetDefaultCamera();

	/// === 軸方向ラインの初期化 === ///

	//3方向分のサイズを確保
	axisLines_.resize(3);

	//X軸ラインの初期化
	axisLines_[0] = std::make_unique<DebugLine>();
	axisLines_[0]->Initialize(transform_.GetRight(), { 1.0f,0.0f,0.0f,1.0f });
	axisLines_[0]->SetParent(&transform_);

	//Y軸ラインの初期化
	axisLines_[1] = std::make_unique<DebugLine>();
	axisLines_[1]->Initialize(transform_.GetUp(), { 0.0f,1.0f,0.0f,1.0f });
	axisLines_[1]->SetParent(&transform_);

	//Z軸ラインの初期化
	axisLines_[2] = std::make_unique<DebugLine>();
	axisLines_[2]->Initialize(transform_.GetForward(), { 0.0f,0.0f,1.0f,1.0f });
	axisLines_[2]->SetParent(&transform_);

	isDebug_ = true;
}

///=====================================================/// 
/// 更新処理
///=====================================================///
void Object3D::Update() {

	transform_.UpdateMatrix();

	//ワールドビュープロジェクション行列
	Matrix4x4 worldViewProjectionMatrix = transform_.GetWorldMatrix();

	if (camera_) {

		/// === カメラ情報があったら === ///

		//カメラのビュープロジェクション行列を取得
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();

		//ワールドビュープロジェクション行列の計算
		worldViewProjectionMatrix *= viewProjectionMatrix;
	}

	//座標変換行列データの設定
	WVPData_->WVP = worldViewProjectionMatrix;
	WVPData_->World = transform_.GetWorldMatrix();
	WVPData_->WorldInverseTranspose = Inverse4x4(transform_.GetWorldMatrix());

	for (auto& line : axisLines_) {
		line->Update();
	}
}

///=====================================================/// 
/// 描画処理
///=====================================================///
void Object3D::Draw(LayerType layer) {

	//Renderクラスに渡す
	std::function<void()> command;

	command = [this]() {

		//3Dオブジェクトの描画前処理
		object3DCommon_->CommonDrawSetting();

		//座標変換行列データの設定
		object3DCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, WVPResource_.Get()->GetGPUVirtualAddress());

		//3Dモデルが割り当てられていれば描画する
		if (model_) {
			model_->Draw();
		}
		};

	Renderer::GetInstance()->AddDraw(layer, command);

	if (isDebug_) {

		command = [this]() {

			for (auto& line : axisLines_) {
				line->Draw();
			}
			};

		Renderer::GetInstance()->AddDraw(Debug, command);

	}
}

void Object3D::DebugDraw() {

	for (auto& line : axisLines_) {
		line->Draw();
	}
}

///=====================================================/// 
/// ImGuiの表示
///=====================================================///
void Object3D::DisplayImGui() {

	Vector4 color = model_->GetColor();

	float shininess = model_->GetShininess();

	transform_.DisplayImGui();

	ImGui::ColorEdit3("Color", &color.x);
	ImGui::DragFloat("Shininess", &shininess, 0.1f);

	model_->SetColor(color);
	model_->SetShininess(shininess);
}

///=====================================================/// 
/// モデルのセッター
///=====================================================///
void Object3D::SetModel(const std::string& modelName) {

	model_ = ModelManager::GetInstance()->FindModel(modelName);
}
#include "DebugObject3D.h"

#include "Base/DirectXCommon.h"
#include "Base/Renderer.h"

#include "3d/Object/DebugObjectCommon.h"
#include "3d/Model/ModelManager.h"
#include "3d/Camera/Camera.h"

#include "Math/MakeMatrixMath.h"

void DebugObject3D::Initialize() {

	//3Dオブジェクト基底のインスタンスを取得
	debugCommon_ = DebugObjectCommon::GetInstance();

	//座標変換行列リソースを作成
	WVPResource_ = debugCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	//書き込むためのアドレスを取得する
	WVPResource_->Map(0, nullptr, reinterpret_cast<void**>(&WVPData_));

	//座標変換行列データの設定
	WVPData_->WVP = MakeIdentity4x4();
	WVPData_->World = MakeIdentity4x4();
	WVPData_->WorldInverseTranspose = MakeIdentity4x4();

	transform_.Initialize();

	//今持っているカメラをデフォルトカメラに設定
	camera_ = debugCommon_->GetCamera();
}

void DebugObject3D::Update() {

	transform_.UpdateMatrix();

	//ワールドビュープロジェクション行列
	Matrix4x4 worldViewProjectionMatrix = transform_.GetWorldMatrix();

	if (camera_) {

		//カメラのビュープロジェクション行列を取得
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();

		//ワールドビュープロジェクション行列の計算
		worldViewProjectionMatrix *= viewProjectionMatrix;
	}

	//座標変換行列データの設定
	WVPData_->WVP = worldViewProjectionMatrix;
	WVPData_->World = transform_.GetWorldMatrix();
	WVPData_->WorldInverseTranspose = Inverse4x4(transform_.GetWorldMatrix());

}

void DebugObject3D::Draw() {

	//Renderクラスに渡す
	std::function<void()> command;

	command = [this]() {

		//3Dオブジェクトの描画前処理
		debugCommon_->CommonDrawSetting();

		//座標変換行列データの設定
		debugCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, WVPResource_.Get()->GetGPUVirtualAddress());

		if (model_) {
			model_->Draw();
		}
		};

	Renderer::GetInstance()->AddDraw(LayerType::Debug, true, command);
}

void DebugObject3D::SetModel(const std::string& modelName) {

	model_ = ModelManager::GetInstance()->FindModel(modelName);
}
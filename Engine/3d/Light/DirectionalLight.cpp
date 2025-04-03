#include "DirectionalLight.h"

#include "Base/DirectXCommon.h"

#include "imgui.h"

///=====================================================/// 
/// 初期化
///=====================================================///
void DirectionalLight::Initialize() {

	//DirectX基底を取得
	dxCommon_ = DirectXCommon::GetInstance();

	//リソースを作成
	lightResource_ = dxCommon_->CreateBufferResource(sizeof(LightData));

	//光源データのアドレスを記録
	lightResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));

	/// === 光源データの設定 === ///

	//色の設定
	lightData_->color = { 1.0f,1.0f,1.0f,1.0f };

	//向きの設定
	lightData_->direction = { 0.0f,-1.0f,0.0f };

	//照度の設定
	lightData_->intensity = 1.0f;
}

///=====================================================/// 
/// 更新
///=====================================================///
void DirectionalLight::Update() {

	//向きを正規化する
	lightData_->direction = Normalize(lightData_->direction);
}

///=====================================================/// 
/// データをGPUに送信
///=====================================================///
void DirectionalLight::SendDataForGPU() {

	//光源データをGPUに送信
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(4, lightResource_.Get()->GetGPUVirtualAddress());
}

///=====================================================/// 
/// ImGuiの表示
///=====================================================///
void DirectionalLight::DisplayImGui() {

	ImGui::Begin("DirectionalLight");

	ImGui::ColorEdit4("Color", &lightData_->color.x);

	ImGui::DragFloat3("Direction", &lightData_->direction.x, 0.01f);

	ImGui::DragFloat("Intensity", &lightData_->intensity, 0.01f);



	ImGui::End();
}
#include "SpotLight.h"

#include "Base/DirectXCommon.h"

#include "imgui.h"

#include "algorithm"

#include "Math/Clamp.h"

///=====================================================/// 
/// 初期化
///=====================================================///
void SpotLight::Initialize() {

	//DirectX基底を取得
	dxCommon_ = DirectXCommon::GetInstance();

	//リソースを作成
	lightResource_ = dxCommon_->CreateBufferResource(sizeof(LightData));

	//光源データのアドレスを記録
	lightResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));

	/// === 光源データの設定 === ///

	//色の設定
	lightData_->color = { 1.0f,1.0f,1.0f,1.0f };

	//座標の設定
	lightData_->position = { 0.0f,0.0f,0.0f };

	//照度の設定
	lightData_->intensity = 1.0f;

	//方向の設定
	lightData_->direction = { 0.0f,0.0f,0.0f };

	//距離の設定
	lightData_->distance = 1.0f;

	//減少度の設定
	lightData_->decay = 1.0f;

	//拡散角度の設定
	lightData_->diffusionAngle = 0.0f;

	//拡散率の設定
	lightData_->diffusionRate = 0.0f;

}

///=====================================================/// 
/// 更新
///=====================================================///
void SpotLight::Update() {

	//照度を0以上に制限
	lightData_->intensity = ClampMinZero(lightData_->intensity);

	//方向を正規化
	lightData_->direction = Normalize(lightData_->direction);

	//距離を0以上に制限
	lightData_->distance = ClampMinZero(lightData_->distance);

	//減衰率を0以上に制限
	lightData_->decay = ClampMinZero(lightData_->decay);

	//拡散角度を0以上に制限
	lightData_->diffusionAngle = std::clamp(lightData_->diffusionAngle, 0.0f, 1.0f);

	//拡散率を0以上に制限
	lightData_->diffusionRate = ClampMinZero(lightData_->diffusionRate);
}

///=====================================================/// 
/// データをGPUに送信
///=====================================================///
void SpotLight::SendDataForGPU() {

	//光源データをGPUに送信
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(6, lightResource_.Get()->GetGPUVirtualAddress());
}

///=====================================================/// 
/// ImGuiの表示
///=====================================================///
void SpotLight::DisplayImGui() {

	ImGui::Begin("SpotLight");

	ImGui::ColorEdit4("Color", &lightData_->color.x);

	ImGui::DragFloat3("Position", &lightData_->position.x, 0.1f);

	ImGui::DragFloat("Intensity", &lightData_->intensity, 0.01f);

	ImGui::DragFloat3("Direction", &lightData_->direction.x, 0.01f);

	ImGui::DragFloat("Distance", &lightData_->distance, 0.01f);

	ImGui::DragFloat("Decay", &lightData_->decay, 0.01f);

	ImGui::DragFloat("Angle", &lightData_->diffusionAngle, 0.01f);

	ImGui::DragFloat("FallOffStart", &lightData_->diffusionRate, 0.01f);

	ImGui::End();
}
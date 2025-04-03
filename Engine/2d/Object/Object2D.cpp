#include "Object2D.h"

#include "Base/WinApp.h"
#include "Base/DirectXCommon.h"
#include "Base/Renderer.h"
#include "2d/Object/Object2DCommon.h"
#include "2d/Sprite/Sprite.h"
#include "2d/Sprite/SpriteManager.h"

#include "Math/MakeMatrixMath.h"

#include "imgui.h"
#include "cassert"

///=====================================================/// 
/// コンストラクタ
///=====================================================///
Object2D::Object2D() {

	//2Dオブジェクト基底のインスタンスを取得
	object2DCommon_ = Object2DCommon::GetInstance();

	//座標変換行列リソースを作成
	WVPResource_ = object2DCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	//書き込むためのアドレスを取得する
	WVPResource_->Map(0, nullptr, reinterpret_cast<void**>(&WVPData_));

	//座標変換行列データの設定
	WVPData_->WVP = MakeIdentity4x4();
	WVPData_->World = MakeIdentity4x4();
	WVPData_->WorldInverseTranspose = MakeIdentity4x4();

	//座標の設定
	translate_ = { 0.0f,0.0f };

	//大きさの設定
	size_ = { 256.0f,256.0f };

	//角度の設定
	rotate_ = 0.0f;
}

///=====================================================/// 
/// 更新
///=====================================================///
void Object2D::Update() {

	//スプライトの更新
	if (sprite_) {
		sprite_->Update();
	}

	//ワールド行列に変換できる形に直す
	Transform transform{
		{size_.x,size_.y,1.0f},
		{0.0f,0.0f,rotate_},
		{translate_.x,translate_.y,0.0f}
	};

	//ワールド行列を生成
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	//ビュー行列を単位行列で生成
	Matrix4x4 viewMatrix = MakeIdentity4x4();

	//プロジェクション行列を生成
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, WinApp::kClientWidth, WinApp::kClientHeight, 0.0f, 100.0f);

	//WVPデータを設定
	WVPData_->WVP = worldMatrix * (viewMatrix * projectionMatrix);
	WVPData_->World = worldMatrix;
	WVPData_->WorldInverseTranspose = Inverse4x4(worldMatrix);
}

///=====================================================/// 
/// 描画
///=====================================================///
void Object2D::Draw(LayerType layer) {

	std::function<void()> func;

	func = [this]() {

		//2Dオブジェクトの描画前処理
		Object2DCommon::GetInstance()->CommonDrawSetting();

		//座標変換行列データの設定
		object2DCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, WVPResource_.Get()->GetGPUVirtualAddress());

		//スプライトが割り当てられていれば描画する
		if (sprite_) {
			sprite_->Draw();
		}
		};

	Renderer::GetInstance()->AddDraw(layer, func);
}

///=====================================================/// 
/// ImGuiの表示
///=====================================================///
void Object2D::DisplayImGui() {

	//スプライトの色を取得
	Vector4 color = sprite_->GetColor();

	//座標
	ImGui::DragFloat3("Translate", &translate_.x, 0.1f);

	//大きさ
	ImGui::DragFloat3("Size", &size_.x, 0.1f);

	//角度
	ImGui::DragFloat("Rotate", &rotate_, 0.1f);

	//色
	ImGui::ColorEdit3("Color", &color.x);

	//色情報を設定
	sprite_->SetColor(color);
}

///=====================================================/// 
/// スプライトのセッター
///=====================================================///
void Object2D::SetSprite(const std::string& spriteName) {

	//引数のスプライト名からスプライトを探す
	sprite_ = SpriteManager::GetInstance()->FindSprite(spriteName);

	size_ = sprite_->GetSize();
}

#include "Object2D.h"

#include "Base/WinApp.h"
#include "Base/DirectXCommon.h"
#include "Base/Renderer.h"

#include "2d/Sprite/SpriteManager.h"

#include <Math/Utility/MakeMatrixMath.h>

#include "imgui.h"
#include "cassert"

namespace MyEngine {

	///=====================================================/// 
	/// Object2Dの初期化
	///=====================================================///
	void Object2D::Initialize() {

		/// === インスタンスの取得 === ///

		//2Dオブジェクト基底のインスタンスを取得
		object2DCommon_ = Object2DCommon::GetInstance();

		/// === 座標変換行列リソースを作成 === ///

		//リソースの作成
		WVPResource_ = object2DCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

		//書き込むためのアドレスを取得する
		WVPResource_->Map(0, nullptr, reinterpret_cast<void**>(&WVPData_));

		//データを初期化
		WVPData_->WVP = MakeIdentity4x4();
		WVPData_->World = MakeIdentity4x4();
		WVPData_->WorldInverseTranspose = MakeIdentity4x4();

		transform_.Initialize();

		/// === その他変数の初期化 === ///

		//座標の設定
		translate_ = { 0.0f,0.0f };

		//大きさの設定
		size_ = { 256.0f,256.0f };

		//角度の設定
		rotate_ = 0.0f;
	}

	///=====================================================/// 
	/// Object2Dのスプライトおよび座標変換行列(WVP)を更新
	///=====================================================///
	void Object2D::Update() {

		/// === スプライトの更新 === ///

		if (sprite_) {
			sprite_->Update();
		}

		/// === 座標変換行列の更新 === ///

		//ワールド行列に変換できる形に直す
		Transform transform{
			{size_.x,size_.y,1.0f},
			{0.0f,0.0f,rotate_},
			{translate_.x,translate_.y,0.0f}
		};

		transform_.translate_ = { translate_.x,translate_.y,0.0f };
		transform_.rotate_ = { 0.0f,0.0f,rotate_ };
		transform_.scale_ = { size_.x,size_.y,1.0f };

		transform_.UpdateMatrix();

		//ワールド行列を生成
		Matrix4x4 worldMatrix = transform_.GetWorldMatrix();

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
	/// Object2Dを指定したレイヤーに描画登録
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

		//レンダラーに描画関数を登録
		Renderer::GetInstance()->AddDraw(layer, true, func);
	}

	///=====================================================/// 
	/// ImGuiを使用してObject2Dのパラメータを表示
	///=====================================================///
	void Object2D::DisplayImGui() {

#ifdef _USE_IMGUI

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

#endif // _USE_IMGUI

	}

	///=====================================================/// 
	/// 名前で指定したスプライトをObject2Dに設定
	///=====================================================///
	void Object2D::SetSprite(const std::string& spriteName) {

		//引数のスプライト名からスプライトを探す
		sprite_ = SpriteManager::GetInstance()->FindSprite(spriteName);

		//スプライトのサイズに合わせる
		size_ = sprite_->GetSize();
	}
}
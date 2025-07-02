#include "SkyBox.h"

#include "Base/DirectXCommon.h"
#include "Base/Renderer.h"
#include "3d/Object/SkyBoxCommon.h"
#include "3d/Camera/Camera.h"
#include "2d/Sprite/TextureManager.h"

#include "Math/MakeMatrixMath.h"

void SkyBox::Initialize(std::string filePath) {

	skyBoxCommon_ = SkyBoxCommon::GetInstance();

	/// === 頂点リソースの生成 === ///

	vertexCount_ = 4 * surfaceCount_;

	//頂点リソースの生成
	vertexResource_ = skyBoxCommon_->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * vertexCount_);

	//頂点バッファビューの作成
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * vertexCount_;

	//1頂点当たりのサイズを設定
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//リソースにデータを書き込めるようにする
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	/// === 頂点データの設定 === ///

	//右面
	vertexData_[0].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[0].texcoord = { 1.0f, 1.0f };
	vertexData_[0].normal = { 0.0f, 0.0f, 1.0f };
	vertexData_[1].position = { 1.0f, 1.0f,-1.0f, 1.0f };
	vertexData_[1].texcoord = { 1.0f, 0.0f };
	vertexData_[1].normal = { 0.0f, 0.0f, 1.0f };
	vertexData_[2].position = { 1.0f,-1.0f, 1.0f, 1.0f };
	vertexData_[2].texcoord = { 0.0f, 1.0f };
	vertexData_[2].normal = { 0.0f, 0.0f, 1.0f };
	vertexData_[3].position = { 1.0f,-1.0f,-1.0f, 1.0f };
	vertexData_[3].texcoord = { 0.0f, 0.0f };
	vertexData_[3].normal = { 0.0f, 0.0f, 1.0f };

	//左面
	vertexData_[4].position = { -1.0f, 1.0f,-1.0f, 1.0f };
	vertexData_[4].texcoord = { 1.0f, 1.0f };
	vertexData_[4].normal = { 0.0f, 0.0f,-1.0f };
	vertexData_[5].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[5].texcoord = { 1.0f, 0.0f };
	vertexData_[5].normal = { 0.0f, 0.0f,-1.0f };
	vertexData_[6].position = { -1.0f,-1.0f,-1.0f, 1.0f };
	vertexData_[6].texcoord = { 0.0f, 1.0f };
	vertexData_[6].normal = { 0.0f, 0.0f,-1.0f };
	vertexData_[7].position = { -1.0f,-1.0f, 1.0f, 1.0f };
	vertexData_[7].texcoord = { 0.0f, 0.0f };
	vertexData_[7].normal = { 0.0f, 0.0f,-1.0f };

	//前面
	vertexData_[8].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[8].texcoord = { 1.0f, 1.0f };
	vertexData_[8].normal = { 0.0f, 1.0f, 0.0f };
	vertexData_[9].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[9].texcoord = { 1.0f, 0.0f };
	vertexData_[9].normal = { 0.0f, 1.0f, 0.0f };
	vertexData_[10].position = { -1.0f,-1.0f, 1.0f, 1.0f };
	vertexData_[10].texcoord = { 0.0f, 1.0f };
	vertexData_[10].normal = { 0.0f, 1.0f, 0.0f };
	vertexData_[11].position = { 1.0f,-1.0f, 1.0f, 1.0f };
	vertexData_[11].texcoord = { 0.0f, 0.0f };
	vertexData_[11].normal = { 0.0f, 1.0f, 0.0f };

	//後面
	vertexData_[12].position = { 1.0f, 1.0f,-1.0f, 1.0f };
	vertexData_[12].texcoord = { 1.0f, 1.0f };
	vertexData_[12].normal = { 0.0f,-1.0f, 0.0f };
	vertexData_[13].position = { -1.0f, 1.0f,-1.0f, 1.0f };
	vertexData_[13].texcoord = { 1.0f, 0.0f };
	vertexData_[13].normal = { 0.0f,-1.0f, 0.0f };
	vertexData_[14].position = { 1.0f,-1.0f,-1.0f, 1.0f };
	vertexData_[14].texcoord = { 0.0f, 1.0f };
	vertexData_[14].normal = { 0.0f,-1.0f, 0.0f };
	vertexData_[15].position = { -1.0f,-1.0f,-1.0f, 1.0f };
	vertexData_[15].texcoord = { 0.0f, 0.0f };
	vertexData_[15].normal = { 0.0f,-1.0f, 0.0f };

	//上面
	vertexData_[16].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[16].texcoord = { 1.0f, 1.0f };
	vertexData_[16].normal = { 0.0f, 0.0f, 1.0f };
	vertexData_[17].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[17].texcoord = { 1.0f, 0.0f };
	vertexData_[17].normal = { 0.0f, 0.0f, 1.0f };
	vertexData_[18].position = { -1.0f, 1.0f,1.0f, 1.0f };
	vertexData_[18].texcoord = { 0.0f, 1.0f };
	vertexData_[18].normal = { 0.0f, 0.0f, 1.0f };
	vertexData_[19].position = { 1.0f, 1.0f,1.0f, 1.0f };
	vertexData_[19].texcoord = { 0.0f, 0.0f };
	vertexData_[19].normal = { 0.0f, 0.0f, 1.0f };

	//下面
	vertexData_[20].position = { -1.0f,-1.0f,1.0f, 1.0f };
	vertexData_[20].texcoord = { 1.0f, 1.0f };
	vertexData_[20].normal = { 0.0f, 0.0f,-1.0f };
	vertexData_[21].position = { 1.0f,-1.0f,1.0f, 1.0f };
	vertexData_[21].texcoord = { 1.0f, 0.0f };
	vertexData_[21].normal = { 0.0f, 0.0f,-1.0f };
	vertexData_[22].position = { -1.0f,-1.0f, -1.0f, 1.0f };
	vertexData_[22].texcoord = { 0.0f, 1.0f };
	vertexData_[22].normal = { 0.0f, 0.0f,-1.0f };
	vertexData_[23].position = { 1.0f,-1.0f, -1.0f, 1.0f };
	vertexData_[23].texcoord = { 0.0f, 0.0f };
	vertexData_[23].normal = { 0.0f, 0.0f,-1.0f };

	/// === 頂点インデックスリソースの生成 === ///

	indexCount_ = 6 * surfaceCount_;

	//頂点インデックスリソースの生成
	IndexResource_ = skyBoxCommon_->GetDxCommon()->CreateBufferResource(sizeof(uint32_t) * indexCount_);

	//リソースの場所を取得
	indexBufferView_.BufferLocation = IndexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズを設定
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * indexCount_;

	//フォーマットを設定
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//リソースにデータを書き込めるようにする
	IndexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	for (uint32_t index = 0; index < surfaceCount_; ++index) {
		indexData_[index * 6 + 0] = index * 4 + 0;
		indexData_[index * 6 + 1] = index * 4 + 1;
		indexData_[index * 6 + 2] = index * 4 + 2;
		indexData_[index * 6 + 3] = index * 4 + 1;
		indexData_[index * 6 + 4] = index * 4 + 3;
		indexData_[index * 6 + 5] = index * 4 + 2;
	}

	/// === マテリアルリソースの生成 === ///

	//マテリアルリソースを作成
	materialResource_ = skyBoxCommon_->GetDxCommon()->CreateBufferResource(sizeof(Material));

	//書き込むためのアドレスを取得する
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	//マテリアルデータの設定
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 50.0f;

	textureFilePath_ = filePath;

	TextureManager::GetInstance()->LoadTexture(textureFilePath_);

	/// === 座標変換行列リソースの生成 === ///

	//座標変換行列リソースを作成
	WVPResource_ = skyBoxCommon_->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

	//書き込むためのアドレスを取得する
	WVPResource_->Map(0, nullptr, reinterpret_cast<void**>(&WVPData_));

	//座標変換行列データの設定
	WVPData_->WVP = MakeIdentity4x4();
	WVPData_->World = MakeIdentity4x4();
	WVPData_->WorldInverseTranspose = MakeIdentity4x4();

	transform_.Initialize();

	//今持っているカメラをデフォルトカメラに設定
	camera_ = skyBoxCommon_->GetCamera();
}

void SkyBox::Update() {

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

void SkyBox::Draw() {

	//Renderクラスに渡す
	std::function<void()> command;

	command = [this]() {

		//3Dオブジェクトの描画前処理
		skyBoxCommon_->CommonDrawSetting();

		//VBVを設定
		skyBoxCommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

		//IBVを設定
		skyBoxCommon_->GetDxCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

		//マテリアルデータの設定
		skyBoxCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());

		//座標変換行列データの設定
		skyBoxCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, WVPResource_.Get()->GetGPUVirtualAddress());

		//テクスチャデータの設定
		skyBoxCommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureFilePath_));

		//描画コマンド発行
		skyBoxCommon_->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(UINT(indexCount_), 1, 0, 0, 0);

		};

	Renderer::GetInstance()->AddDraw(LayerType::Object, true, command);
}
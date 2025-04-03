#include "TextureManager.h"

#include "Base/SrvManager.h"

#include "Other/Log.h"

#include "cassert"

//SRVの総数を1に設定
uint32_t TextureManager::kSRVIndexTop = 1;

///=====================================================/// 
/// シングルトンインスタンス
///=====================================================///
TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;
	return &instance;
}

///=====================================================/// 
/// 初期化処理
///=====================================================///
void TextureManager::Initialize() {

	//DirectX基底のインスタンスを取得
	directXCommon_ = DirectXCommon::GetInstance();

	//SRVマネージャーのインスタンスを取得
	srvManager_ = SrvManager::GetInstance();

	//テクスチャデータの最大数を設定
	textureDatas.reserve(srvManager_->kMaxSRVCount_);
}

///=====================================================/// 
/// テクスチャの読み込み
///=====================================================///
void TextureManager::LoadTexture(const std::string& filePath) {

	//読み込み済みテクスチャを検索
	if (textureDatas.contains(filePath)) {

		//読み込み済みなら早期return
		return;
	}

	//テクスチャ枚数上限チェック
	assert(srvManager_->AllocateCheck());

	//テクスチャファイルを読み込んで、プログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの生成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//テクスチャデータのメモリの場所を取得
	TextureData& textureData = textureDatas[filePath];

	//テクスチャデータの内容を設定
	textureData.modelName = filePath;
	textureData.metaData = mipImages.GetMetadata();
	textureData.resource = CreateTextureResource(textureData.metaData);
	textureData.intermediateResource = UploadTextureData(textureData.resource, mipImages);
	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

	//metadataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureData.metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(textureData.metaData.mipLevels);

	directXCommon_->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
}

///=====================================================/// 
/// メタデータのゲッター
///=====================================================///
const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& filePath) {

	TextureData& textureData = textureDatas[filePath];

	return textureData.metaData;
}

///=====================================================/// 
/// SRV番号のゲッター
///=====================================================///
uint32_t TextureManager::GetSrvIndex(const std::string& filePath) {

	TextureData& textureData = textureDatas[filePath];

	return textureData.srvIndex;
}

///=====================================================/// 
/// GPUデスクリプタのゲッター
///=====================================================///
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& filePath) {

	TextureData& textureData = textureDatas[filePath];

	return textureData.srvHandleGPU;
}

///=====================================================/// 
/// テクスチャリソースの生成
///=====================================================///
Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata) {

	//metadataをもとにResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);                             //Textureの幅
	resourceDesc.Height = UINT(metadata.height);                           //Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);                   //mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);            //奥引き or 配列Textureの配列数
	resourceDesc.Format = metadata.format;                                 //TextureのFormat
	resourceDesc.SampleDesc.Count = 1;                                     //サンプリングカウント 1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension); //Textureの次元数。普段使っているのは2次元

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; //デフォルト

	//Resourceの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = directXCommon_->GetDevice()->CreateCommittedResource(
		&heapProperties,                //Heapの設定
		D3D12_HEAP_FLAG_NONE,           //Heapの特殊な設定。特になし
		&resourceDesc,                  //Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST, //初回のResourceState。Textureは基本読むだけ
		nullptr,                        //Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource)         //作成するResourceポインタへのポインタ
	);

	//生成できたかの確認
	assert(SUCCEEDED(hr));

	return resource;
}

///=====================================================/// 
/// テクスチャデータをGPUに転送
///=====================================================///
[[nodiscard]]
Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::UploadTextureData(
	Microsoft::WRL::ComPtr<ID3D12Resource> textureData,
	const DirectX::ScratchImage& mipImages
) {

	//GPUに送るためのリソース
	std::vector<D3D12_SUBRESOURCE_DATA> subResources;

	//サブリソースの生成
	DirectX::PrepareUpload(
		directXCommon_->GetDevice(), //デバイス
		mipImages.GetImages(),       //イメージ
		mipImages.GetImageCount(),   //イメージ数
		mipImages.GetMetadata(),     //メタデータ
		subResources                 //サブリソース
	);

	//中間リソースに必要なサイズを計算
	uint64_t intermediateSize = GetRequiredIntermediateSize(
		textureData.Get(),        //アップロード先のリソース
		0,                        //最初のサブリソースのインデックス
		UINT(subResources.size()) //サブリソースの数
	);

	//中間リソースの場所を作成
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = directXCommon_->CreateBufferResource(intermediateSize);

	//中間リソースをサブリソースにコピー
	UpdateSubresources(
		directXCommon_->GetCommandList(), //コマンドリスト
		textureData.Get(),                //コピー先のテクスチャリソース
		intermediateResource.Get(),       //コピー元の中間リソース
		0,                                //中間リソースの開始オフセット
		0,                                //最初のサブリソースインデックス
		UINT(subResources.size()),        //サブリソースの数
		subResources.data()               //サブリソースデータの配列
	);

	//リソースバリアの初期化
	D3D12_RESOURCE_BARRIER barrier{};

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;                    //タイプはTRANSISION
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;                         //フラグなし
	barrier.Transition.pResource = textureData.Get();                         //遷移対象のリソース
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES; //全サブリソース
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;          //コピー先の状態
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;        //読み取り可能な状態に遷移

	//コマンドリストにリソースバリアを設定
	directXCommon_->GetCommandList()->ResourceBarrier(1, &barrier);

	return intermediateResource;
}
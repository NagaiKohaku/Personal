#include "TextureManager.h"


#include "Other/Log.h"

#include "cassert"

namespace MyEngine {

	//SRVの総数を1で初期化
	uint32_t TextureManager::kSRVIndexTop = 1;

	///=====================================================/// 
	/// TextureManagerのシングルトンインスタンスを取得
	///=====================================================///
	TextureManager* TextureManager::GetInstance() {
		static TextureManager instance;
		return &instance;
	}

	///=====================================================/// 
	/// TextureManagerを初期化
	///=====================================================///
	void TextureManager::Initialize(DirectXCommon* dxCommonPtr, SRVManager* srvPtr) {

		//DirectX基底のインスタンスを取得
		dxCommon_ = dxCommonPtr;

		//SRVマネージャーのインスタンスを取得
		srvManager_ = srvPtr;

		//テクスチャデータの最大数を設定
		textureData_.reserve(srvManager_->kMaxSRVCount_);
	}

	///=====================================================/// 
	/// 指定されたファイルパスのテクスチャを読み込み、GPU上に展開
	///=====================================================///
	void TextureManager::LoadTexture(const std::string& filePath) {

		if (filePath == "") {

			return;
		}

		//読み込み済みテクスチャを検索
		if (textureData_.contains(filePath)) {

			//読み込み済みなら早期return
			return;
		}

		//テクスチャ枚数上限チェック
		assert(srvManager_->AllocateCheck());

		/// === テクスチャファイルを読み込んで、プログラムで扱えるようにする === ///

		DirectX::ScratchImage image{};

		//ファイルパスをワイド文字列に変換
		std::wstring filePathW = ConvertString(filePath);

		HRESULT hr;

		//PNGファイルの読み込み
		hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);

		assert(SUCCEEDED(hr));

		//ミップマップの生成
		DirectX::ScratchImage mipImages{};
		if (DirectX::IsCompressed(image.GetMetadata().format)) {

			//圧縮テクスチャの場合は、ミップマップを生成せずにそのまま使用
			mipImages = std::move(image);
		} else {

			//非圧縮テクスチャの場合は、ミップマップを生成
			hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
		}
		assert(SUCCEEDED(hr));

		//テクスチャデータのメモリの場所を取得
		TextureData& textureData = textureData_[filePath];

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

		//SRVの生成
		dxCommon_->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
	}

	///==============================================================/// 
	/// 指定されたファイルパスのキューブマップテクスチャを読み込み、GPU上に展開
	///==============================================================///
	void TextureManager::LoadCubeTexture(const std::string& filePath) {

		//読み込み済みテクスチャを検索
		if (textureData_.contains(filePath)) {

			//読み込み済みなら早期return
			return;
		}

		//テクスチャ枚数上限チェック
		assert(srvManager_->AllocateCheck());

		/// === テクスチャファイルを読み込んで、プログラムで扱えるようにする === ///

		DirectX::ScratchImage image{};

		//ファイルパスをワイド文字列に変換
		std::wstring filePathW = ConvertString(filePath);

		HRESULT hr;

		if (filePathW.ends_with(L".dds")) {

			//DDSファイルの読み込み
			hr = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
		} else {

			//PNGファイルの読み込み
			hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
		}
		assert(SUCCEEDED(hr));

		//ミップマップの生成
		DirectX::ScratchImage mipImages{};
		if (DirectX::IsCompressed(image.GetMetadata().format)) {

			//圧縮テクスチャの場合は、ミップマップを生成せずにそのまま使用
			mipImages = std::move(image);
		} else {

			//非圧縮テクスチャの場合は、ミップマップを生成
			hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
		}
		assert(SUCCEEDED(hr));

		//テクスチャデータのメモリの場所を取得
		TextureData& textureData = textureData_[filePath];

		//テクスチャデータの内容を設定
		textureData.modelName = filePath;
		textureData.metaData = mipImages.GetMetadata();
		textureData.srvIndex = srvManager_->Allocate();
		textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
		textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

		if (filePathW.ends_with(L".dds")) {

			textureData.resource = CreateTextureResource(textureData.metaData);
			textureData.intermediateResource = UploadTextureData(textureData.resource, mipImages);
		} else {

			textureData.resource = CreateCubeTextureResource(textureData.metaData);
			textureData.intermediateResource = UploadCubeTextureData(textureData.resource, mipImages);
		}

		//metadataを基にSRVの設定
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

		srvDesc.Format = textureData.metaData.format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = UINT_MAX;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;

		//SRVの生成
		dxCommon_->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
	}

	///=====================================================/// 
	/// メタデータを取得
	///=====================================================///
	const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& filePath) {

		TextureData& textureData = textureData_[filePath];

		return textureData.metaData;
	}

	///=====================================================/// 
	/// SRV番号を取得
	///=====================================================///
	uint32_t TextureManager::GetSrvIndex(const std::string& filePath) {

		if (textureData_.find(filePath) == textureData_.end()) {

			return textureData_["Resource/Texture/white_128x128.png"].srvIndex;
		}

		return textureData_[filePath].srvIndex;
	}

	///=====================================================/// 
	/// GPUデスクリプタを取得
	///=====================================================///
	D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& filePath) {

		if (textureData_.find(filePath) == textureData_.end()) {

			return textureData_["Resource/Texture/white_128x128.png"].srvHandleGPU;
		}

		return textureData_[filePath].srvHandleGPU;
	}

	///===================================================================/// 
	/// 指定されたテクスチャメタデータを基に、DirectX 12 のテクスチャリソースを生成
	///===================================================================///
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

		HRESULT hr = dxCommon_->GetDevice()->CreateCommittedResource(
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

	///===================================================================/// 
	/// 指定されたメタデータを基に、キューブマップ用のテクスチャリソースを生成
	///===================================================================///
	Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateCubeTextureResource(const DirectX::TexMetadata& metadata) {

		D3D12_RESOURCE_DESC resourceDesc = {};

		resourceDesc.Width = metadata.width;
		resourceDesc.Height = static_cast<UINT>(metadata.height);
		resourceDesc.MipLevels = 1;
		resourceDesc.DepthOrArraySize = 6; // キューブマップは6面
		resourceDesc.Format = metadata.format;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		//利用するHeapの設定
		D3D12_HEAP_PROPERTIES heapProperties{};

		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; //デフォルト

		Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;

		HRESULT hr = dxCommon_->GetDevice()->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&resource));

		//生成できたかの確認
		assert(SUCCEEDED(hr));

		return resource;
	}

	///=========================================================/// 
	/// テクスチャデータをGPUにアップロードし、GPUで使用可能な状態にする
	///=========================================================///
	[[nodiscard]]
	Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::UploadTextureData(
		Microsoft::WRL::ComPtr<ID3D12Resource> textureData,
		const DirectX::ScratchImage& mipImages
	) {

		//GPUに送るためのリソース
		std::vector<D3D12_SUBRESOURCE_DATA> subResources;

		//サブリソースの生成
		DirectX::PrepareUpload(
			dxCommon_->GetDevice(), //デバイス
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
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = dxCommon_->CreateBufferResource(intermediateSize);

		//中間リソースをサブリソースにコピー
		UpdateSubresources(
			dxCommon_->GetCommandList(), //コマンドリスト
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
		dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);

		return intermediateResource;
	}

	///==================================================================/// 
	/// キューブマップ用のテクスチャデータをGPUにアップロードし、使用可能な状態にする
	///==================================================================///
	[[nodiscard]]
	Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::UploadCubeTextureData(
		Microsoft::WRL::ComPtr<ID3D12Resource> textureData,
		const DirectX::ScratchImage& mipImages
	) {

		const DirectX::Image* img = mipImages.GetImage(0, 0, 0);

		// アップロードバッファ作成
		size_t uploadBufferSize = GetRequiredIntermediateSize(textureData.Get(), 0, 6);

		Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;

		//利用するHeapの設定
		D3D12_HEAP_PROPERTIES heapProperties{};

		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

		CD3DX12_RESOURCE_DESC resourceDesc{};

		resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

		dxCommon_->GetDevice()->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&intermediateResource));

		// 6面へコピー用のSubresourceDataを作成
		std::vector<D3D12_SUBRESOURCE_DATA> subresources(6);

		for (int i = 0; i < 6; ++i) {

			subresources[i].pData = img->pixels;

			subresources[i].RowPitch = img->rowPitch;

			subresources[i].SlicePitch = img->slicePitch;
		}

		UpdateSubresources(
			dxCommon_->GetCommandList(),
			textureData.Get(),
			intermediateResource.Get(),
			0,
			0,
			6,
			subresources.data()
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
		dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);

		return intermediateResource;
	}
}
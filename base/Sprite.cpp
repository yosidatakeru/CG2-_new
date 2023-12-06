#include "Sprite.h"
#include "Base.h"

void Sprite::Initialize(DirectXCommon* directXCommon, SpriteCommon* spriteCommon)
{
	directXCommon_ = directXCommon;
	spriteCommon_ = spriteCommon;
	vertexResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(VertexData) * 3);
	
	//Resourceにデータを書き込む
	materialResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(Vector4) * 3); ;

	////Resourceにデータを書き込む
	wvpResource = CreateBufferResource(directXCommon->GetDevice(), sizeof(Matrix4x4)); ;







#pragma region  VertexBufferViewを作成
	////VertexBufferViewを作成
	//頂点バッファビューを作成する
	
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(Vector4) * 3;
	//１頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(Vector4);



}



void Sprite::Update(Transform transform, Transform cameraTransform)
{



	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	*wvpData = worldViewProjectionMatrix;
}




void Sprite::Draw(DirectXCommon* directXCommon)
{
	directXCommon_ = directXCommon;

#pragma region VertexResourceを生成する

#pragma endregion



#pragma region マテリアル用Resourceにデータを書き込む

	//マテリアルにデータを書き込む
	Vector4* materialData = nullptr;

	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	//今回は赤を書き込む(ここで色を変えられる)
	*materialData = Vector4(1.0f, 0.0f, 0.0f, 1.0f);


#pragma endregion



#pragma endregion


	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));


	*wvpData = MakeIdentity4x4();

	//新しく引数作った方が良いかも


#pragma region 



#pragma endregion

#pragma region Resourceにデータを書き込む
	//Resourceにデータを書き込む
    VertexData* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[0].texCoord = { 0.0f,1.0f };

	//上
	vertexData[1].position = { 0.0f,0.5f,0.0f,1.0f };
	vertexData[1].texCoord = { 0.5f,0.0f };

	//右下
	vertexData[2].position = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData[2].texCoord = { 1.0f,1.0f };
#pragma endregion


#pragma region ViewportとScissor


	

#pragma endregion


#pragma region コマンドを積む
	directXCommon->GetCommandList()->RSSetViewports(1, directXCommon->GetViewport()); //&viewport);

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXCommon->GetCommandList()->SetGraphicsRootSignature(spriteCommon_->GetRootSignature());
	directXCommon->GetCommandList()->SetPipelineState(spriteCommon_->GetGraphicsPipelineState());
	directXCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	//wvp用のCBufferの場所を設定
	directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());

	//描画(DrawCall)３兆点で１つのインスタンス。
	directXCommon->GetCommandList()->DrawInstanced(3, 1, 0, 0);
	

#pragma endregion
}

void Sprite::Releases()
{
	vertexResource->Release();
	materialResource->Release();
	wvpResource->Release();
}







void Sprite::LoadTexture(const std::string& filePath)
{
	DirectX::ScratchImage mipImages = LoadTextureData("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	ID3D12Resource* textureResource = CreateTextureResource(directXCommon_->GetDevice(), metadata);
	UploadTextureData(textureResource, mipImages);


	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = 
	directXCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU =
    directXCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();


	//先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU.ptr += directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	//SRVの生成
	directXCommon_->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);
}



ID3D12Resource* Sprite::CreateBufferResource(ID3D12Device* device, size_t sizeInbyte)
{
	HRESULT hr{};
	ID3D12Resource* RssultResource;
	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};

	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; //UploadHeapを使う

	//頂点リソースの設定
	D3D12_RESOURCE_DESC ResourceDesc{};


	//バッファリソース。テクスチャの場合はまた別の設定をする
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeInbyte; //リソースのサイズ。今回はvector4を3頂点分

	//バッファの場合はこれらは1にする決まり
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;

	//バッファの場合はこれにする決まり
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	
	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&RssultResource));
	assert(SUCCEEDED(hr));

	return RssultResource;
}


//Textureを読み込むためのLoad関数
//1.TextureデータそのものをCPUで読み込む

DirectX::ScratchImage Sprite::LoadTextureData(const std::string& filePath)
{
	HRESULT hr{};
	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの作成
	//ミップマップ...元画像より小さなテクスチャ群
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//ミップマップ月のデータを返す
	return mipImages;
}

ID3D12Resource* Sprite::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
{
	//1.metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	//Textureの幅
	resourceDesc.Width = UINT(metadata.width);
	//Textureの高さ
	resourceDesc.Height = UINT(metadata.height);
	//mipmapの数
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	//奥行き or 配列Textureの配列数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	//TextureのFormat
	resourceDesc.Format = metadata.format;
	//サンプリングカウント
	resourceDesc.SampleDesc.Count = 1;
	//Textureの次元数。普段使っているのは2次元
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

	//2.利用するHeapの設定
	//利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース版がある
	D3D12_HEAP_PROPERTIES heapProperties{};
	//細かい設定を行う
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	//WriteBackポリシーでCPUアクセス可能
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	//プロセッサの近くに配置
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;


	HRESULT hr{};
	//3.Resourceを生成する
	ID3D12Resource* resource = nullptr;
	hr= directXCommon_->GetDevice()->CreateCommittedResource(
		&heapProperties,					//Heapの設定
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_GENERIC_READ,	//初回のResourceState。Textureは基本読むだけ
		nullptr,							//Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource));			//作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));

	return resource;


}

void Sprite::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	HRESULT hr{};
	//Meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();

	//全MipMapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel)
	{
		//MipMapLevelを指定して各Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		//Textureに転送
		hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,				//全領域
			img->pixels,			//元データアドレス
			UINT(img->rowPitch),	//1ラインサイズ
			UINT(img->slicePitch));	//1枚サイズ

		assert(SUCCEEDED(hr));
	}

}






#include"Sprite2d.h"

void Sprite2d::Initialize(DirectXCommon* directXCommon, SpriteCommon* spriteCommon)
{
	directXCommon_ = directXCommon;
	spriteCommon_ = spriteCommon;


	////画像読み込み
	DirectX::ScratchImage mipImages = spriteCommon->LoadTexture(L"Resources/uvChecker.png");
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	textureResource = CreateTextureResource(directXCommon_->GetDevice(), metaData);
	spriteCommon_->UploadTewtureData(textureResource, mipImages);

	wvpResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(TransformationMatrix));
}



void Sprite2d::Update(Transform transform, Transform cameraTransform, Transform trnsformSprite)
{
	////Sprite用のWorlsViewProjectionMatrixを作る
	/*Matrix4x4 worudMatrixSprite = MakeAffineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worudMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));

	*transformationMatrixDataSprite = worldViewProjectionMatrixSprite;*/
}

void Sprite2d::Draw(DirectXCommon* directXCommon)
{

}

ID3D12Resource* Sprite2d::CreateBufferResource(ID3D12Device* device, size_t sizeInByte)
{
	ID3D12Resource* resource = nullptr;
	////VertexResourceを生成
	//頂点リソース用のヒープを設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInByte;
	//バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際に頂点リソースを作る

	HRESULT	hr = device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));


	return  0;

}

ID3D12Resource* Sprite2d::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metaData)
{

		//1.metadataを基にResourceの設定
		D3D12_RESOURCE_DESC resourceDesc{};
		//Textureの幅
		resourceDesc.Width = UINT(metaData.width);
		//Textureの高さ
		resourceDesc.Height = UINT(metaData.height);
		//mipmapの数
		resourceDesc.MipLevels = UINT16(metaData.mipLevels);
		//奥行き or 配列Textureの配列数
		resourceDesc.DepthOrArraySize = UINT16(metaData.arraySize);
		//TextureのFormat
		resourceDesc.Format = metaData.format;
		//サンプリングカウント
		resourceDesc.SampleDesc.Count = 1;
		//Textureの次元数。普段使っているのは2次元
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metaData.dimension);

		//2.利用するHeapの設定
		//利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース版がある
		D3D12_HEAP_PROPERTIES heapProperties{};
		//細かい設定を行う
		heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
		//WriteBackポリシーでCPUアクセス可能
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		//プロセッサの近くに配置
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;



		//3.Resourceを生成する
		ID3D12Resource* resource = nullptr;
		HRESULT	hr = device->CreateCommittedResource(
			&heapProperties,					//Heapの設定
			D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定
			&resourceDesc,						//Resourceの設定
			D3D12_RESOURCE_STATE_GENERIC_READ,	//初回のResourceState。Textureは基本読むだけ
			nullptr,							//Clear最適値。使わないのでnullptr
			IID_PPV_ARGS(&resource));			//作成するResourceポインタへのポインタ
		assert(SUCCEEDED(hr));

		return resource;
}



void Sprite2d::CreateVertex()
{
	//Sprite用のの頂点リソースを作る
	vertexResourceSprite = CreateBufferResource(directXCommon_->GetDevice(), sizeof(VertexData) * 6);


	//リソースの先頭のアドレス
	vertexbufferViewSprite.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点6つぶんのサイズ
	vertexbufferViewSprite.SizeInBytes = sizeof(VertexData) * 6;

	//１頂点当たりのサイズ
	vertexbufferViewSprite.StrideInBytes = sizeof(VertexData);

	//頂点データの設定
	//解放処理していない

	vertexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));

	vertexDataSprite[0].position = {0.0f,360.0f,0.0f,1.0f };
	vertexDataSprite[0].texcoord = { 0.0f,1.0f };
	////上
	vertexDataSprite[1].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexDataSprite[1].texcoord = { 0.0f,0.0f };
	////右下
	vertexDataSprite[2].position = { 640.0f,360.0f,0.0f,1.0f };
	vertexDataSprite[2].texcoord = { 1.0f,1.0f };


	////左下
	vertexDataSprite[3].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexDataSprite[3].texcoord = { 0.0f,0.0f };
	////上
	vertexDataSprite[4].position = { 640.0f,0.0f,0.0f,1.0f };
	vertexDataSprite[4].texcoord = { 1.0f,0.0f };
	////右下
	vertexDataSprite[5].position = { 640.0f,360.0f,0.0f,1.0f };
	vertexDataSprite[5].texcoord = { 1.0f,1.0f };



}

void Sprite2d::Releases()
{

		//indexResourceSprite->Release();
		//directionalLighlResource->Release();
		//vertexResource->Release();
		//materialResource->Release();
		//wvpResource->Release();
		//textureResource->Release();
		//textureResource2->Release();
	
}

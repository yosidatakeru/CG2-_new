#pragma once
#include<d3d12.h>
#include <cassert>

ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInbyte)
{
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
	HRESULT hr;
	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&RssultResource));
	assert(SUCCEEDED(hr));

	return RssultResource;
}

//テクスチャリソースを作成する関数
ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metaData)
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

};
#include "Triangle.h"
#include"externals/DirectXTex/d3dx12.h"
#include "Buff.h"


void Triangle::Initialize(DirectXCommon* directXCommon)
{
	directXCommon_ = directXCommon;

	CreateVertex();

	CreateMAterial();

	CreateWVP();

}

void Triangle::CreateVertex()
{
	//VertexBufferViewを作成
	//頂点バッファビューを作成する

//	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexResouce = CreateBufferResource(sizeof(VertexData) * 6);
	
}

void Triangle::CreateMAterial()
{
	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource = CreateBufferResource(sizeof(Material));
}

void Triangle::CreateWVP()
{
}

ID3D12Resource* Triangle::CreateBufferResource(size_t sizeInBytes)
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
	vertexResourceDesc.Width = sizeInBytes;
	//バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際に頂点リソースを作る

	HRESULT	hr = directXCommon_->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));


	return  resource;
}

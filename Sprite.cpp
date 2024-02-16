#include "Sprite.h"
#include"Bufftr.h"
void Sprite::Initialize(DirectXCommon* directXCommon, SpriteCommon* spriteCommon)
{
	directXCommon_ = directXCommon;
	spriteCommon_ = spriteCommon;

	////画像読み込み
	DirectX::ScratchImage mipImages = spriteCommon->LoadTexture(L"Resources/uvChecker.png");
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	textureResource = CreateTextureResource(directXCommon_->GetDevice(), metaData);
	spriteCommon_->UploadTewtureData(textureResource, mipImages);
	

	
	

	////SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metaData.mipLevels);

	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU =
		directXCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU =
		directXCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();


	textureSrvHandleCPU.ptr += directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);



	//SRVの生成
	directXCommon_->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);

	CreateVertex();

	CreateMAterial();

	CreateWVP();



	
	



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


	transform_.translate = { position.x,position.y, 0 };
	transform_.rotate = { rotation,0,0 };

	
	
	*materialData = color_;

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

	directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);

	//描画(DrawCall)３兆点で１つのインスタンス。
	directXCommon->GetCommandList()->DrawInstanced(3, 1, 0, 0);



#pragma endregion
}




void Sprite::Releases()
{
	vertexResource->Release();
	materialResource->Release();
	wvpResource->Release();
	textureResource->Release();
}




void Sprite::CreateVertex()
{
	////VertexBufferViewを作成
	//頂点バッファビューを作成する
	vertexResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(VertexData) * 3);

	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 3;
	//１頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);


	//Resourceにデータを書き込む
	
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	//上
	vertexData[1].position = { 0.0f,0.5f,0.0f,1.0f };
	vertexData[1].texcoord = { 0.5f,0.0f };
	//右下
	vertexData[2].position = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };
}



void Sprite::CreateMAterial()
{
	
	//Resourceにデータを書き込む
	materialResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(Vector4) * 3); ;


	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	
	*materialData = color_;
}



void Sprite::CreateWVP()
{
	////Resourceにデータを書き込む
	wvpResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(Matrix4x4)); ;
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));


	*wvpData = MakeIdentity4x4();

	
}







ID3D12Resource* Sprite::CreateBufferResource(ID3D12Device* device, size_t sizeInbyte)
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

#include "Sprite.h"

void Sprite::Initialize(DirectXCommon* directXCommon, SpriteCommon* spriteCommon)
{
	directXCommon_ = directXCommon;
	spriteCommon_ = spriteCommon;
	vertexResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(Vector4) * 3);
	
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
	Vector4* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0] = { -0.5f,-0.5f,0.0f,1.0f };
	//上
	vertexData[1] = { 0.0f,0.5f,0.0f,1.0f };
	//右下
	vertexData[2] = { 0.5f,-0.5f,0.0f,1.0f };

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

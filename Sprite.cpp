#include "Sprite.h"

void Sprite::Initialize(DirectXCommon* directXCommon, SpriteCommon* spriteCommon)
{
	directXCommon_ = directXCommon;
	spriteCommon_ = spriteCommon;
	
	
	

#pragma region 



#pragma endregion




#pragma region ViewportとScissor
	CreateVertex();
	const uint32_t kSubdivision = 12;
	const uint32_t kNumSphereVerices = kSubdivision * kSubdivision * 6;
	float pi = std::numbers::pi_v<float>;

	CreateMAterial();

	CreateWVP();

}







	




void Sprite::Draw(Transform transform, Transform cameraTransform)
{
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	*wvpData = worldViewProjectionMatrix;


#pragma region VertexResourceを生成する

#pragma endregion









#pragma endregion


#pragma region コマンドを積む
	directXCommon_->GetCommandList()->RSSetViewports(1, directXCommon_->GetViewport()); //&viewport);

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXCommon_->GetCommandList()->SetGraphicsRootSignature(spriteCommon_->GetRootSignature());
	directXCommon_->GetCommandList()->SetPipelineState(spriteCommon_->GetGraphicsPipelineState());
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	//wvp用のCBufferの場所を設定
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());

	//描画(DrawCall)３兆点で１つのインスタンス。
	directXCommon_->GetCommandList()->DrawInstanced(kNumSphereVerices, 1, 0, 0);



#pragma endregion
}




void Sprite::Releases()
{
	vertexResource->Release();
	materialResource->Release();
	wvpResource->Release();
}

void Sprite::CreateVertex()
{
	////VertexBufferViewを作成
	//頂点バッファビューを作成する
	vertexResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(VertexData) * kNumSphereVerices );

	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * kNumSphereVerices;
	//１頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);


#pragma region Resourceにデータを書き込む
	//Resourceにデータを書き込む
	VertexData* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	//vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	//vertexData[0].texcoord = { 0.0f,1.0f };

	////上
	//vertexData[1].position = { 0.0f,0.5f,0.0f,1.0f };
	//vertexData[1].texcoord = { 0.5f,0.0f };

	////右下
	//vertexData[2].position = { 0.5f,-0.5f,0.0f,1.0f };
	//vertexData[2].texcoord = { 1.0f,1.0f };

	const float kLonEvery = pi * 2.0f / float(kSubdivision);
	const float kLatEvery = pi / float(kSubdivision);

	for (uint32_t latlndex = 0;latlndex  < kSubdivision; ++latlndex)
	{
		float lat = -pi / 2.0f + kLatEvery * latlndex;
	
	
		for (uint32_t lonlndex = 0; lonlndex < kSubdivision; ++lonlndex) {

			uint32_t startlndex = (latlndex * kSubdivision + lonlndex) * 6;

			float lon = lonlndex * kLonEvery;


			vertexData[startlndex].position.x = std::cos(lat) * std::cos(lon);
			vertexData[startlndex].position.y = std::sin(lat);
			vertexData[startlndex].position.z = std::cos(lat) * std::sin(lon);
			vertexData[startlndex].position.w = 1.0f;
			vertexData[startlndex].texcoord =
			{ float(lonlndex) / float(kSubdivision), 1.0f - float(latlndex) / float(kSubdivision) };


			vertexData[startlndex + 1].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
			vertexData[startlndex + 1].position.y = std::sin(lat + kLatEvery);
			vertexData[startlndex + 1].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
			vertexData[startlndex + 1].position.w = 1.0f;
            vertexData[startlndex + 1].texcoord =
			{ float(lonlndex) / float(kSubdivision), 1.0f - float(latlndex + 1) / float(kSubdivision) };


			vertexData[startlndex + 2].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
			vertexData[startlndex + 2].position.y = std::sin(lat);
			vertexData[startlndex + 2].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
			vertexData[startlndex + 2].position.w = 1.0f;
			vertexData[startlndex + 2].texcoord =
			{ float(lonlndex + 1) / float(kSubdivision), 1.0f - float(latlndex) / float(kSubdivision) };
		


			vertexData[startlndex + 3] = vertexData[startlndex + 2];
			vertexData[startlndex + 4] = vertexData[startlndex + 1];
			
			vertexData[startlndex + 5].position.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
			vertexData[startlndex + 5].position.y = std::sin(lat + kLatEvery);
			vertexData[startlndex + 5].position.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
			vertexData[startlndex + 5].position.w = 1.0f;
			vertexData[startlndex + 5].texcoord =
			{ float(lonlndex + 1) / float(kSubdivision),1.0f - float(latlndex + 1) / float(kSubdivision) };

		}

	}
#pragma endregion


}


void Sprite::CreateMAterial()
{
#pragma region  VertexBufferViewを作成

#pragma region マテリアル用Resourceにデータを書き込む
	//Resourceにデータを書き込む
	materialResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(Vector4) * 3); ;


	//マテリアルにデータを書き込む
	Vector4* materialData = nullptr;

	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	//今回は赤を書き込む(ここで色を変えられる)
	*materialData = Vector4(1.0f, 0.0f, 0.0f, 1.0f);


#pragma endregion



}







void Sprite::CreateWVP()
{

	////Resourceにデータを書き込む
	wvpResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(Matrix4x4)); ;




	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));


	*wvpData = MakeIdentity4x4();

	//新しく引数作った方が良いかも

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

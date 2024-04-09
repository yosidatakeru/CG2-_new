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
	const uint32_t kSubdivision = 12;
	const uint32_t kNumSphereVerices = kSubdivision * kSubdivision * 6;
	float pi = std::numbers::pi_v<float>;



	CreateMAterial();

	CreateWVP();


	CreateTransform();
	
	



}



void Sprite::Update(Transform transform, Transform cameraTransform, Transform transformSprite)
{



	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	*wvpData = worldViewProjectionMatrix;

	//Sprite用のWorlsViewProjectionMatrixを作る
	Matrix4x4 worudMatrixSprite = MakeAffineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worudMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));

	*transformationMatrixDataSprite = worldViewProjectionMatrixSprite;

}




void Sprite::Draw(DirectXCommon* directXCommon)
{
	directXCommon_ = directXCommon;


	transform_.translate = { position.x,position.y, 0 };
	//回転パラメータ
	transform_.rotate = { 0,rotation,0 };

	
	
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
	directXCommon->GetCommandList()->DrawInstanced(kNumSphereVerices, 1, 0, 0);



	//Spriteの描画変更が必要なものだけ変更
	//追加
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexbufferViewSprite);//VBVの設定

	//TransformationMatrionMatrixCBufferの場所を設定
	directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite->GetGPUVirtualAddress());

	//描画(DrawCall)３兆点で１つのインスタンス。
	directXCommon->GetCommandList()->DrawInstanced(6, 1, 0, 0);



#pragma endregion
}




void Sprite::Releases()
{
	transformationMatrixResourceSprite->Release();
	vertexResourceSprite->Release();
	vertexResource->Release();
	materialResource->Release();
	wvpResource->Release();
	textureResource->Release();
}




void Sprite::CreateVertex()
{
	////VertexBufferViewを作成
	//頂点バッファビューを作成する
	vertexResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(VertexData) * kNumSphereVerices);

	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * kNumSphereVerices;
	//１頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);


	////Resourceにデータを書き込む
	//
	////書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	////左下
	//vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	//vertexData[0].texcoord = { 0.0f,1.0f };
	////上
	//vertexData[1].position = { 0.0f,0.5f,0.0f,1.0f };
	//vertexData[1].texcoord = { 0.5f,0.0f };
	////右下
	//vertexData[2].position = { 0.5f,-0.5f,0.0f,1.0f };
	//vertexData[2].texcoord = { 1.0f,1.0f };


	////左下
	//vertexData[3].position = { -0.5f,-0.5f,0.5f,1.0f };
	//vertexData[3].texcoord = { 0.0f,1.0f };
	////上
	//vertexData[4].position = { 0.0f,0.0f,0.0f,1.0f };
	//vertexData[4].texcoord = { 0.5f,0.0f };
	////右下
	//vertexData[5].position = { 0.5f,-0.5f,-0.5f,1.0f };
	//vertexData[5].texcoord = { 1.0f,1.0f };



	const float kLonEvery = pi * 2.0f / float(kSubdivision);
	const float kLatEvery = pi / float(kSubdivision);

	for (uint32_t latlndex = 0; latlndex < kSubdivision; ++latlndex)
	{
		float lat = -pi / 2.0f + kLatEvery * latlndex;


		for (uint32_t lonlndex = 0; lonlndex < kSubdivision; ++lonlndex)
		{

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
	//上
	vertexDataSprite[1].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexDataSprite[1].texcoord = { 0.0f,0.0f };
	//右下
	vertexDataSprite[2].position = { 640.0f,360.0f,0.0f,1.0f };
	vertexDataSprite[2].texcoord = { 1.0f,1.0f };


	//左下
	vertexDataSprite[3].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexDataSprite[3].texcoord = { 0.0f,0.0f };
	//上
	vertexDataSprite[4].position = { 640.0f,0.0f,0.0f,1.0f };
	vertexDataSprite[4].texcoord = { 1.0f,0.0f };
	//右下
	vertexDataSprite[5].position = { 640.0f,360.0f,0.0f,1.0f };
	vertexDataSprite[5].texcoord = { 1.0f,1.0f };



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

void Sprite::CreateTransform()
{
	//Sprite用のTransformationMatrix用のリソースを作るMatrix4x4 1とつぶんのサイズを用意する
	transformationMatrixResourceSprite = CreateBufferResource(directXCommon_->GetDevice(), sizeof(Matrix4x4));
	
	

	//書き込みのためのアドレスを取得
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));

	//単位行列を書き込む
	*transformationMatrixDataSprite = MakeIdentity4x4();
	
}








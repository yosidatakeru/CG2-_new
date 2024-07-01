#include "Sprite.h"
#include "Base.h"
#include "TextureManager.h"

void Sprite::Initialize(DirectXCommon* directXCommon, SpriteCommon* spriteCommon, std::wstring textureFilePath)
{
	directXCommon_ = directXCommon;
	spriteCommon_ = spriteCommon;
	//モデル読み込み
	
	 
	textureIndex = TextureManager::GetInstance()->GetTextureIndexFilePath(textureFilePath);



	CreateVertex();
	
	//CreateTexture(textureFilePath);

	CreateMAterial();

	CreateWVP();

	CreateTransform();
	
	CreatLight();
	



}



void Sprite::Update(Transform transform, Transform cameraTransform, Transform transformSprite)
{




	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	wvpData->WVP = worldViewProjectionMatrix;
	wvpData->World = worldMatrix;


	//Sprite用のWorlsViewProjectionMatrixを作る
	Matrix4x4 worudMatrixSprite = MakeAffineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worudMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));

	*transformationMatrixDataSprite = worldViewProjectionMatrixSprite;



	//////Sprite用のWorlsViewProjectionMatrixを作る
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransformSprite.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateXMatrix(uvTransformSprite.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));
	materialDataSprit->uvTrasform = uvTransformMatrix;


	//ImGui::Checkbox("useMonsterBall", &useMonsterBall);


	ImGui::Begin("texture");
	ImGui::DragFloat3("light", &light.x, 0.01f, -1.0f, 1.0f);


	//ImGui::Begin("model");

	//ImGui::DragFloat3("model", &rotation, 1.0f, -1.0f, 3.0f);

	ImGui::DragFloat3("model", &position.x, 1.0f, -1.0f, 1000.0f);

	//ImGui::DragFloat3("model", &size.x, 1.0f, -1.0f, 1000.0f);


	ImGui::End();

	ImGui::Checkbox("uvTrasform", &uvSprite);


	ImGui::Begin("texture");
	ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);


	ImGui::End();


	directionalLighlData->direction = light;




	transformSprite_.translate = { position.x,position.y, 0 };
	//回転パラメータ
	transformSprite_.rotate = { 0,rotation,0 };



	materialDataSprit->color = color_;

	//サイズ
	transformSprite_.scale = { size.x, size.y, 1.0f };

}




void Sprite::Draw(DirectXCommon* directXCommon)
{
	directXCommon_ = directXCommon;
#pragma region コマンドを積む
	




	directXCommon->GetCommandList()->RSSetViewports(1, directXCommon->GetViewport());

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXCommon->GetCommandList()->SetGraphicsRootSignature(spriteCommon_->GetRootSignature());
	directXCommon->GetCommandList()->SetPipelineState(spriteCommon_->GetGraphicsPipelineState());
	//スプライト
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexbufferViewSprite);//VBVの設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directXCommon->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);//IBVを設定
	//マテリアルCBufferの場所を設定
	directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprit->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
    ////TransformationMatrionMatrixCBufferの場所を設定
	directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite->GetGPUVirtualAddress());
	directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetStvHandleGPU(textureIndex));
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLighlResource->GetGPUVirtualAddress());
	////描画(DrawCall)３兆点で１つのインスタンス。
	directXCommon->GetCommandList()->DrawInstanced(6, 1, 0, 0);


#pragma endregion
}




void Sprite::Releases()
{
	//indexResourceSprite->Release();
	directionalLighlResource->Release();
	transformationMatrixResourceSprite->Release();
	vertexResourceSprite->Release();
	//vertexResource->Release();
	materialResourceSprit->Release();
	materialResource->Release();
	wvpResource->Release();
	//textureResource->Release();
	//textureResource2->Release();
}




void Sprite::CreateVertex()
{

	

	
	
	//Sprite用のの頂点リソースを作る
	vertexResourceSprite = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(VertexData) * 6);

	//リソースの先頭のアドレス
	vertexbufferViewSprite.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点6つぶんのサイズ
	vertexbufferViewSprite.SizeInBytes = sizeof(VertexData) * 6;

	//１頂点当たりのサイズ
	vertexbufferViewSprite.StrideInBytes = sizeof(VertexData);

	//頂点データの設定
	//解放処理していない

	vertexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));

	vertexDataSprite[0].position = { 0.0f,360.0f,0.0f,1.0f };
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
	materialResource = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(Material) ); ;


	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color_;
	materialData->enableLighting = false;
	materialData->uvTrasform = MakeIdentity4x4();



	//Resourceにデータを書き込む
	materialResourceSprit = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(Material)); ;
	//書き込むためのアドレスを取得
	materialResourceSprit->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprit));
	materialDataSprit->color = color_;
	materialDataSprit->enableLighting = false;
	materialDataSprit->uvTrasform = MakeIdentity4x4();

	
	
	
}



void Sprite::CreateWVP()
{
	////Resourceにデータを書き込む
	wvpResource = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(TransformationMatrix)); ;
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));


	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();

	
}

void Sprite::CreateTransform()
{
	//Sprite用のTransformationMatrix用のリソースを作るMatrix4x4 1とつぶんのサイズを用意する
	transformationMatrixResourceSprite = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(TransformationMatrix));
	
	

	//書き込みのためのアドレスを取得
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));

	//単位行列を書き込む
	*transformationMatrixDataSprite = MakeIdentity4x4();
	
}

void Sprite::CreatLight()
{
	////Resourceにデータを書き込む
	directionalLighlResource = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(DirectionalLigha)); ;

	// directionalLighlData = nullptr;

	//書き込むためのアドレスを取得
	directionalLighlResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLighlData));

	directionalLighlData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLighlData->direction = { 0.0f, -1.0f, 0.0f };
	directionalLighlData->intensity = 1.0f;

}

void Sprite::CreateTexture(std::wstring textureFilePath)
{
	
	////画像読み込み
	DirectX::ScratchImage mipImages = spriteCommon_->LoadTexture(textureFilePath);
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	textureResource = spriteCommon_->CreateTextureResource(directXCommon_->GetDevice(), metaData);

	//画像データを送る
	ID3D12Resource* texture = spriteCommon_->GetIntermediateResource();
	texture = spriteCommon_->UploadTewtureData(textureResource, mipImages);
	spriteCommon_->SetIntermediateResource(texture);


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


}



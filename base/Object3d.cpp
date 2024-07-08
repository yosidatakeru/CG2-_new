#include "Object3d.h"
#include "Base.h"
#include<cassert>
#include"Object3dCommon.h"
#include"TextureManager.h"

void Object3d::Initialize(DirectXCommon* directXCommon, Object3dCommon* Object3dCommon)
{
	directXCommon_ = directXCommon;
    object3dCommon_ = Object3dCommon;


	modelData = model_->LoadObjFile("Resources", "plane.obj");
	
	CreateWVP();

	
	//CreateTransform();

	//CreatLight();*/

}

void Object3d::Update(Transform transform, Transform cameraTransform, Transform transformSprite)
{

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	wvpData->WVP = worldViewProjectionMatrix;
	wvpData->World = worldMatrix;



	ImGui::Begin("texture");
	//ImGui::DragFloat3("light", &light.x, 0.01f, -1.0f, 1.0f);


	ImGui::Begin("model");

	ImGui::DragFloat3("model", &rotation, 1.0f, -1.0f, 3.0f);

	//ImGui::DragFloat3("model", &position.x, 1.0f, -1.0f, 1000.0f);

	//ImGui::DragFloat3("model", &size.x, 1.0f, -1.0f, 1000.0f);


	ImGui::End();

	
	
	


	ImGui::End();


}



void Object3d::Draw(DirectXCommon* directXCommon)
{

	transform_.translate = { 0,position.y, 0 };
	//回転パラメータ
	transform_.rotate = { 0,rotation,0 };

	//materialData->color = color_;


	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	
	directXCommon_ = directXCommon;
	
	
	
		
	

	
	//wvp用のCBufferの場所を設定
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());



	//if (model_) 
	//{
		model_->Draw();
	//}
	
	//directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLighlResource->GetGPUVirtualAddress());



	



}

void Object3d::Releases()
{
//	vertexResource->Release();
	
	
	wvpResource->Release();
	
	//textureResource->Release();
}







void Object3d::CreatTexture(std::wstring filePath)
{
	//////画像読み込み
	
	DirectX::ScratchImage mipImages = object3dCommon_->LoadTexture(filePath);
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	textureResource = object3dCommon_->CreateTextureResource(directXCommon_->GetDevice(), metaData);

	//画像データを送る
	ID3D12Resource* texture = object3dCommon_->GetIntermediateResource();
	texture = object3dCommon_->UploadTewtureData(textureResource, mipImages);
	object3dCommon_->SetIntermediateResource(texture);


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








void Object3d::CreateWVP()
{
	////Resourceにデータを書き込む
	wvpResource = object3dCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(TransformationMatrix)); ;
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));


	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();
}




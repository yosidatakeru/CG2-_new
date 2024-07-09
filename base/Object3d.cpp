#include "Object3d.h"
#include "Base.h"
#include<cassert>
#include"Object3dCommon.h"
#include"TextureManager.h"

void Object3d::Initialize(DirectXCommon* directXCommon, Object3dCommon* Object3dCommon)
{
	directXCommon_ = directXCommon;
    object3dCommon_ = Object3dCommon;
	
	
	
	
	CreateWVP();
}



void Object3d::Update(Transform transform, Transform cameraTransform)
{

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	wvpData->WVP = worldViewProjectionMatrix;
	wvpData->World = worldMatrix;

	//ImGui::Begin("texture");
	////ImGui::DragFloat3("light", &light.x, 0.01f, -1.0f, 1.0f);


	//ImGui::Begin("model");

	//ImGui::DragFloat3("model", &rotation, 1.0f, -1.0f, 3.0f);

	//ImGui::DragFloat3("model", &position.x, 1.0f, -1.0f, 1000.0f);

	////ImGui::DragFloat3("model", &size.x, 1.0f, -1.0f, 1000.0f);


	//ImGui::End();


	//ImGui::End();


}



void Object3d::Draw(DirectXCommon* directXCommon)
{

	
	directXCommon_ = directXCommon;
	
	//wvp用のCBufferの場所を設定
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());

	//3Dモデルが割り当てられいれば描画
	//割り当て方が分からん

	if (model_) 
	{
		model_->Draw();
	}
	
	//directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLighlResource->GetGPUVirtualAddress())

}



void Object3d::Releases()
{
	
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




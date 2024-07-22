#include "Object3d.h"
#include "engine/mata/Base.h"
#include<cassert>
#include"Object3dCommon.h"
#include"engine/mata/TextureManager.h"
#include"ModelManager.h"

void Object3d::Initialize(Object3dCommon* Object3dCommon)
{
	this->object3dCommon_ = Object3dCommon;
	
	
	
	
	CreateWVP();
}



void Object3d::Update(Transform transform)
{
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	
	Matrix4x4 viewMatrix = camera.ViewMatrix();
	Matrix4x4 projectionMatrix = camera.ProjectionMatrix();
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	wvpData->WVP = worldViewProjectionMatrix;
	wvpData->World = worldMatrix;

}



void Object3d::Draw()
{
	
	//wvp用のCBufferの場所を設定
	object3dCommon_->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());

	//3Dモデルが割り当てられいれば描画
	//割り当て方が分からん

	if (model_) 
	{
		model_->Draw();
	}
	
	//directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLighlResource->GetGPUVirtualAddress())

}

void Object3d::CameraUpdate(Vector3 cameraPosintion, Vector3 cameraRotation)
{
	
	camera.SetCameraPosintion(cameraPosintion);
	camera.SetCameraRotaion(cameraRotation);
	camera.CameraUpdate();
	
}



void Object3d::Releases()
{
	
}



void Object3d::CreateWVP()
{
	////Resourceにデータを書き込む
	wvpResource = object3dCommon_->CreateBufferResource(object3dCommon_->GetDirectXCommon()->GetDevice(), sizeof(TransformationMatrix)); ;
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();
}

void Object3d::SetModels(const std::string& filePath)
{
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}




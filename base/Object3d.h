#pragma once
#include "MaterialData.h"
#include "ModelData.h"
#include "DirectXCommon.h"
#include"Model.h"

#include"Vector2.h"
#include"Vector3.h"
#include<numbers>
#include"Matrix3x3.h"
#include"ModelData.h"
#include"MaterialData.h"
#include"TransformationMatrix.h"

#include"externals/DirectXTex/DirectXTex.h"




class  Object3dCommon;




class Object3d
{
public:
	void Initialize(Object3dCommon* object3dCommonme);


	void Draw();
	void Update(Transform transform, Transform cameraTransform);

	void Releases();
	

private:
	//行列情報の作成
	void CreateWVP();
			
	//void CreatLight();
public:

	ID3D12Resource* GetwvpResource() const { return  wvpResource.Get(); }
	
	

	Vector3 GetCameraPosition() { return camerPosition; }
	void SetCameraPosintion(Vector3 cameraPos) { camerPosition = cameraPos; }

	Transform GetCameraTransform() { return  cameraTransform_; }

	void SetModels(const std::string& filePath);

	void SetModel(Model* model) { model_ = model; }

private:





	Transform cameraTransform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-120.0f} };

	

	
	Vector3 camerPosition = { 0, 0, 0 };

private:

	ModelCommon* modelCommon = nullptr;

	
	Object3dCommon* object3dCommon_ = nullptr;
	Model* model_ = nullptr;
	
	ModelData modelData;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ID3D12Resource* textureResource;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	DirectX::ScratchImage mipImages;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource = nullptr;

	TransformationMatrix* wvpData = nullptr;

	
	MatrialData* textureIndex;

};

#pragma once
#include"engine/mata/MaterialData.h"
#include"engine/mata/ModelData.h"
#include"engine/base/DirectXCommon.h"
#include"Model.h"

#include"engine/mata/Vector2.h"
#include"engine/mata/Vector3.h"
#include<numbers>
#include"engine/mata/Matrix3x3.h"
#include"engine/mata/ModelData.h"
#include"engine/mata/MaterialData.h"
#include"engine/mata/Transform.h"

#include"engine/base/externals/DirectXTex/DirectXTex.h"
#include "engine/mata/TransformationMatrix.h"
#include"engine/base/Camera.h"



class  Object3dCommon; 




class Object3d:public Camera
{
public:
	void Initialize(Object3dCommon* object3dCommonme);


	void Draw();
	void CameraUpdate(Vector3 pos,Vector3 cameraRotation);

	void Update(Transform transform );

	void Releases();
	

private:
	//行列情報の作成
	void CreateWVP();
			
	//void CreatLight();
public:

	ID3D12Resource* GetwvpResource() const { return  wvpResource.Get(); }
	
	

	//GetCameraPosition() { return camerPosition; }
	//void SetCameraPosintion(Vector3 cameraPos) { camerPosition = cameraPos; }

	//Transform GetCameraTransform() { return  cameraTransform_; }

	void SetModels(const std::string& filePath);

	void SetModel(Model* model) { model_ = model; }

private:





	

	
	Vector3 camerPosition = { 0, 0, 0 };

private:

	ModelCommon* modelCommon = nullptr;
	
	Camera camera;
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

#pragma once
#include "MaterialData.h"
#include "ModelData.h"
#include "DirectXCommon.h"
#include"Model.h"

#include"Vector2.h"
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
	void Initialize(DirectXCommon* directXCommon, Object3dCommon* object3dCommonme);


	void Draw(DirectXCommon* directXCommon);
	void Update(Transform transform, Transform cameraTransform, Transform transformSprite);

	void Releases();
	

private:
	//行列情報の作成
	void CreateWVP();
			
	//void CreatLight();
public:

	ID3D12Resource* GetwvpResource() const { return  wvpResource.Get(); }
	//回転
	float GetRotation() { return rotation; }
	void SetRotaion(float rot) { rotation = rot; }

	//移動
	Vector2 GetPosition() { return position; }
	void SetPosintion(Vector2 pos) { position = pos; }

	Transform GetTransform() { return  transform_; }

	Vector3 GetCameraPosition() { return camerPosition; }
	void SetCameraPosintion(Vector3 cameraPos) { camerPosition = cameraPos; }

	Transform GetCameraTransform() { return  cameraTransform_; }

	Transform GetTransformSprite() { return  transformSprite; }

	void SetModel(Model* model) { model_ = model; }

private:


	float rotation = 0;

	
	Transform transform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	Transform cameraTransform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };

	Transform transformSprite{ {1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };


	Vector2 position = { 0, 0 };

	Vector3 camerPosition = { 0, 0, 0 };

private:

	ModelCommon* modelCommon = nullptr;

	DirectXCommon* directXCommon_ = nullptr;
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

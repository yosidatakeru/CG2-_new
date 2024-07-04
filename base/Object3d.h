#pragma once
#include "MaterialData.h"
#include "ModelData.h"
#include "DirectXCommon.h"
#include "Object3dCommon.h"

#include"Vector2.h"
#include<numbers>
#include"Matrix3x3.h"
#include"Material.h"
#include"VertexData.h"
#include"ModelData.h"
#include"MaterialData.h"


#include<fstream>
#include<sstream>

struct TransformationMatrix
{
	Matrix4x4 WVP;
	Matrix4x4 World;
};

class Object3d
{
public:
	void Initialize(DirectXCommon* directXCommon, Object3dCommon* object3dCommon_);


	void Draw(DirectXCommon* directXCommon);
	void Update(Transform transform, Transform cameraTransform, Transform transformSprite);

	void Releases();
private:

	//頂点データの作成関数
	void CreateVertex();

	//マテリアルの作成関数
	void CreateMAterial();

	//行列情報の作成
	//マテリアルの作成関数
	void CreateWVP();
			
	//void CreatLight();

	//画像データ作成
	void CreatTexture(std::wstring filePath);

	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	MatrialData LoadMatrialTemplateFile(const std::string& directoryPath, const std::string& filename);

public:

	ID3D12Resource* GetwvpResource() const { return  wvpResource; }

	//色
	Vector4 GetColor() { return color_; }
	void SetColor(Vector4 color) { color_ = color; }

	//回転
	float GetRotation() { return rotation; }
	void SetRotaion(float rot) { rotation = rot; }

	//移動
	Vector2 GetPosition() { return position; }
	void SetPosintion(Vector2 pos) { position = pos; }

	Vector3 GetCameraPosition() { return camerPosition; }
	void SetCameraPosintion(Vector3 cameraPos) { camerPosition = cameraPos; }

	Transform GetTransform() { return  transform_; }

	Transform GetCameraTransform() { return  cameraTransform_; }

	Transform GetTransformSprite() { return  transformSprite; }


private:


	float rotation = 0;

	//色パラメーター
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };


	Transform transform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	Transform cameraTransform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };

	Transform transformSprite{ {1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };


	Vector2 position = { 0, 0 };

	Vector3 camerPosition = { 0, 0, 0 };



private:

	DirectXCommon* directXCommon_ = nullptr;
	Object3dCommon* object3dCommon_ = nullptr;

	ID3D12Resource* vertexResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	VertexData* vertexData = nullptr;


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ID3D12Resource* textureResource;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	DirectX::ScratchImage mipImages;

	ID3D12Resource* materialResource = nullptr;
	Material* materialData = nullptr;

	ID3D12Resource* wvpResource = nullptr;

	TransformationMatrix* wvpData = nullptr;

	ModelData modelData; // 構築するMaterualData



};
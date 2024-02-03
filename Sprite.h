#pragma once
#include"base/SpriteCommon.h"
#include"base/DirectXCommon.h"
#include<DirectXMath.h>
#include<numbers>
class Sprite
{
public:
	struct  VertexData
	{
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct Material
	{
		Vector4 color;
		int32_t enableLighting;
		float shininess;
	};

	struct TransformationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};
	
	struct DirectionalLigha
	{
		Vector4 color;//ライトの色
		Vector3 direction;//!<ライトの向き
		float intensity; //!<輝度
	};

	struct CameraForGPU
	{
		Vector3 WorldPosition;
	};


	void Initialize(DirectXCommon* directXCommon , SpriteCommon* SpriteCommon_ );

	void Draw(Transform transform, Transform cameraTransform);
	
	
	void Releases();
	
	ID3D12Resource* GetwvpResource() const { return  wvpResource; }

private:
	//頂点データの作成関数
	void CreateVertex();

	//マテリアルの作成関数
	void CreateMAterial();

	//行列情報の作成
	//マテリアルの作成関数
	void CreateWVP();

	void CreatLight();

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInbyte);

	
	TransformationMatrix* wvpData = nullptr;

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXCommon* directXCommon_ = nullptr;
	ID3D12Resource* vertexResource = nullptr;

	ID3D12Resource* materialResource = nullptr;

	ID3D12Resource* wvpResource = nullptr;
	ID3D12Resource* directionalLighlResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	const uint32_t kSubdivision = 12;
	const uint32_t kNumSphereVerices = kSubdivision * kSubdivision * 6;
	float pi = std::numbers::pi_v<float>;
	Vector3 light = { 0.0f, -1.0f,0.0f };
	DirectionalLigha* directionalLighlData = nullptr;

	ID3D12Resource* cameraResource=nullptr;
	CameraForGPU* cameraData = nullptr;
};


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

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInbyte);

	
	Matrix4x4* wvpData = nullptr;

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXCommon* directXCommon_ = nullptr;
	ID3D12Resource* vertexResource = nullptr;

	ID3D12Resource* materialResource = nullptr;

	ID3D12Resource* wvpResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	const uint32_t kSubdivision = 12;
	const uint32_t kNumSphereVerices = kSubdivision * kSubdivision * 6;
	float pi = std::numbers::pi_v<float>;
};


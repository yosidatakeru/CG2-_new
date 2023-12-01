#pragma once
#include"base/SpriteCommon.h"
#include"base/DirectXCommon.h"
class Sprite
{
public:
	void Initialize(DirectXCommon* directXCommon , SpriteCommon* SpriteCommon_ );

	void Draw(DirectXCommon* directXCommon);
	void Update(Transform transform, Transform cameraTransform);
	
	void Releases();
	
	ID3D12Resource* GetwvpResource() const { return  wvpResource; }

private:
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInbyte);

	
	Matrix4x4* wvpData = nullptr;

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXCommon* directXCommon_ = nullptr;
	ID3D12Resource* vertexResource = nullptr;

	ID3D12Resource* materialResource = nullptr;

	ID3D12Resource* wvpResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};


};


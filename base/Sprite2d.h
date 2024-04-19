#pragma once
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include"TransformationMatrix.h"
#include"VertexData.h"

class Sprite2d
{
public:
	void Initialize(DirectXCommon* directXCommon, SpriteCommon* SpriteCommon_);
	void Update(Transform transform, Transform cameraTransform, Transform trnsformSprite);
	void Draw(DirectXCommon* directXCommon);
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInByte);


	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metaData);
	

	void CreateVertex();
	void Releases();
	
private:
	ID3D12Resource* wvpResource = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	DirectXCommon* directXCommon_ = nullptr;
	ID3D12Resource* textureResource = nullptr;
	VertexData* vertexDataSprite = nullptr;
	ID3D12Resource* vertexResourceSprite = nullptr;
	//頂点バッファリソーソを作る
	D3D12_VERTEX_BUFFER_VIEW vertexbufferViewSprite{};


};


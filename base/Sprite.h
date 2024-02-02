#pragma once
#include"SpriteCommon.h"
#include"DirectXCommon.h"
#include"externals/DirectXTex/DirectXTex.h"
#include"VertexData.h"
#include<wrl.h>
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
	
	//1.TextureデータそのものをCPUで読み込む
	DirectX::ScratchImage LoadTextureData(const std::string& filePath);
	
	//2.DirectX12のTextureResourceを作る
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	//3.TextureResourceに1で読んだデータを転送する
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	void LoadTexture(const std::string& filePath);


	Matrix4x4* wvpData = nullptr;

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXCommon* directXCommon_ = nullptr;
	ID3D12Resource* vertexResource = nullptr;

	ID3D12Resource* materialResource = nullptr;

	ID3D12Resource* wvpResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	struct TransformationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	TransformationMatrix* instancingData = nullptr;
	const uint32_t kNumInstance = 10;
	 
	Transform transfoms[10] ;
	Microsoft::WRL::ComPtr<ID3D12Resource>instancingResource;
};


#pragma once
#include"DirectXCommon.h"
class SrvManager
{
public:
	//初期化
	void Initialize(DirectXCommon* directXCommon);

	void PreDrew();

	void SetGreaphicsRootDescriptorTadle(UINT RootParmeterIndex, uint32_t srvIndex);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	//SRV生成(テクスチャ用)
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);

	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structreByteStride);


	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>  GetDescriptorHeap() { return descriptorHeap.Get(); }

	static const uint32_t kMaxcount;

private:
	uint32_t Allocate();

private:
	uint32_t useIndex = 0;


private:
	DirectXCommon* directXCommon_ = nullptr;

	uint32_t descriptorSize;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;

	//ID3D12DescriptorHeap* CreateDescriptorHeap
	//(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);


};


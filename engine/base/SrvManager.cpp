#include "SrvManager.h"
const uint32_t SrvManager::kMaxcount = 512;

void SrvManager::Initialize(DirectXCommon* directXCommon)
{
    directXCommon_ = directXCommon;
    descriptorHeap = directXCommon_->CreateDescriptorHeap(directXCommon_->GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxcount, true);;
    descriptorSize = directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}

void SrvManager::PreDrew()
{
    ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap.Get() };
    directXCommon_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void SrvManager::SetGreaphicsRootDescriptorTadle(UINT RootParmeterIndex, uint32_t srvIndex)
{
    directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(RootParmeterIndex, GetGPUDescriptorHandle(srvIndex));
}

D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index)
{
    D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    handleCPU.ptr += (descriptorSize * index);
    return handleCPU;

}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index)
{
    D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    handleGPU.ptr += (descriptorSize * index);
    return handleGPU;
}

void SrvManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels)
{
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    
    
    srvDesc.Format = Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
    srvDesc.Texture2D.MipLevels = UINT(MipLevels);
    ////SRVの生成
    directXCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structreByteStride)
{
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.FirstElement = 0;
    srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    srvDesc.Buffer.NumElements = numElements;
    srvDesc.Buffer.StructureByteStride = structreByteStride;

    directXCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));

   
}

uint32_t SrvManager::Allocate()
{
    //return する番号を一旦記録しておく
    int index = useIndex;
    
    //次回のために番号を1進める
    useIndex++;

    //上で記録した番号をreturnする
    return index;

}


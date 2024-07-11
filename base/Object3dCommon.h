#pragma once
#include"DirectXCommon.h"
#include "externals/DirectXTex/DirectXTex.h"


class Object3dCommon
{
public: //メンバ関数
	//初期化
	void Initialize(DirectXCommon* directxCommon);
	
	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

	void DXCInitialize();

	void Object3dPreDraw();
	//リリース
	void Releases();

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInByte);


	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metaData);

	DirectX::ScratchImage LoadTexture(const std::wstring& filePath);
	
	ID3D12Resource* UploadTewtureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);


public:
	DirectXCommon* GetDirectXCommon() const { return directXCommon_; }
	
	ID3D12Resource* GetIntermediateResource() { return  intermediateResource_; }
	void SetIntermediateResource(ID3D12Resource* intermediateResource) { intermediateResource_ = intermediateResource; }


private:

	

	void RootSignature();
    
	void GraphicsPipeline();

	

	ID3D12RootSignature* GetRootSignature() const { return rootSignature.Get(); }

	ID3D12PipelineState* GetGraphicsPipelineState() const { return graphicsPipelineState; }

private:
	DirectXCommon* directXCommon_ = nullptr;

	//ルートシグネチャ
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;

	IDxcBlob* vertexShaderBlob = nullptr;

	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;
	//DXcommonから持ってきた
	IDxcUtils* dxcUtils = nullptr;




	//実際に生成
	ID3D12PipelineState* graphicsPipelineState = nullptr;


	
	

	IDxcBlob* pixelShaderBlob = nullptr;

	



	


	Matrix4x4* wvpData = nullptr;
	Transform* transform = nullptr;

	ID3D12Resource* intermediateResource_ = nullptr;





};


#pragma once
#include"DirectXCommon.h"
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

public:
	DirectXCommon* GetdirectxCommon() const { return directXCommon_; }



private:

	

	void RootSignature();
    
	void GraphicsPipeline();

	ID3D12RootSignature* GetRootSignature() const { return rootSignature.Get(); }

	ID3D12PipelineState* GetGraphicsPipelineState() const { return graphicsPipelineState; }

private:
	DirectXCommon* directXCommon_;

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


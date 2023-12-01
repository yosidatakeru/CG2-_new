#pragma once
#include <dxgidebug.h>
#include <dxcapi.h>
#include"DirectXCommon.h"

class SpriteCommon
{
public:
	//初期化
	void Initialize(DirectXCommon* directXCommon);

	//更新
	//void Update(Transform transform, Transform cameraTransform);

	void Draw();

	void Releases();;



	ID3D12RootSignature* GetRootSignature() const { return rootSignature; }

	ID3D12PipelineState* GetGraphicsPipelineState() const {return graphicsPipelineState;}

private:
	void PsoGenerate();

	void DXCInitialize();
	////CompileShader関数
	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);
private:
  DirectXCommon* directXCommon = nullptr;

  IDxcCompiler3* dxcCompiler = nullptr;
  IDxcIncludeHandler* includeHandler = nullptr;
  //DXcommonから持ってきた
  IDxcUtils* dxcUtils = nullptr;
  

  

  //実際に生成
  ID3D12PipelineState* graphicsPipelineState = nullptr;


  //シリアライズしてバイナリにする
  ID3DBlob* signatureBlob = nullptr;
  ID3DBlob* errorBlob = nullptr;

  //バイナリを元に生成
  ID3D12RootSignature* rootSignature = nullptr;

  IDxcBlob* pixelShaderBlob = nullptr;

  IDxcBlob* vertexShaderBlob = nullptr;

  D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

 
  //D3D12_VIEWPORT viewport{};
 

 Matrix4x4* wvpData = nullptr;
 Transform* transform=nullptr;
 

 


};


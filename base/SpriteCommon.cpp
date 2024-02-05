#include "SpriteCommon.h"
#include <cassert>
#include"Base.h"




void SpriteCommon::Initialize(DirectXCommon* directXCommon)
{
	directXCommon_ = directXCommon;
	HRESULT hr{};
	ComPtr<IDxcUtils>dxcUtils;
	ComPtr<IDxcCompiler3>dxcCompiler;
	ComPtr<IDxcIncludeHandler>includeHandler;
	DXCInitialize();
	PsoGenerate();
}





void SpriteCommon::DXCInitialize()
{
	
	HRESULT hr{};
	

#pragma region DXCの初期化
	//dxcCompilerを初期化
	
	
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr));


	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));

#pragma endregion


}

void SpriteCommon::PsoGenerate()
{
	HRESULT hr{};
	#pragma region RootSignatureを作成
		//RootSignature・・ShaderとResourceをどのように間レンズけるかを示したオブジェクトである
		;
		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0;
		descriptorRange[0].NumDescriptors = 1;
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


		//Material設定
		D3D12_ROOT_PARAMETER rootParameters[5] = {};
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[0].Descriptor.ShaderRegister = 0;
		
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		rootParameters[1].Descriptor.ShaderRegister = 0;
		
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	    rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
		
		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[3].Descriptor.ShaderRegister = 1;

		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[4].Descriptor.ShaderRegister = 2;

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);





		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
		if (FAILED(hr)) {
			Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
			assert(false);
		}
		
		hr = directXCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
			signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		assert(SUCCEEDED(hr));
	
	#pragma endregion

	#pragma region InputLayoutの設定を行う
		//InputLayout
		
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElementDescs[1].SemanticName = "TEXCOORD";
		inputElementDescs[1].SemanticIndex = 0;
		inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElementDescs[2].SemanticName = "NORMAL";
		inputElementDescs[2].SemanticIndex = 0;
		inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
		inputLayoutDesc.pInputElementDescs = inputElementDescs;
		inputLayoutDesc.NumElements = _countof(inputElementDescs);
	#pragma endregion


	#pragma region BlendStateの設定を行う
		////BlendStateの設定を行う
		//BlendStateの設定
		D3D12_BLEND_DESC blendDesc{};
		//全ての色要素を書き込む
		blendDesc.RenderTarget[0].RenderTargetWriteMask =
			D3D12_COLOR_WRITE_ENABLE_ALL;
		
		blendDesc.RenderTarget[0].BlendEnable = TRUE;

		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;


		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;


		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;


		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;


		blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;


		blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;



	#pragma endregion

	#pragma region RasterizerStateの設定を行う
		//RasterizerStateの設定
		D3D12_RASTERIZER_DESC rasterizerDesc{};
		//裏面(時計回り)を表示しない
		rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		//三角形の中を塗りつぶす
		rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	#pragma endregion
		

	#pragma region ShaderをCompileする
		//ShaderをCompileする
		 vertexShaderBlob = CompileShader(L"Object3D.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
		assert(vertexShaderBlob != nullptr);
	
		 pixelShaderBlob = CompileShader(L"Object3D.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
		assert(pixelShaderBlob != nullptr);
	#pragma endregion


		#pragma region PSOを生成
			////PSO生成
			D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
			graphicsPipelineStateDesc.pRootSignature = rootSignature;
			graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
			graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };
			//vertexShaderBlob->GetBufferSize();
			graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };
			pixelShaderBlob->GetBufferSize();
			graphicsPipelineStateDesc.BlendState = blendDesc;
			graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
			//書き込むRTVの情報
			graphicsPipelineStateDesc.NumRenderTargets = 1;
			graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			//利用するトポロジ(形状)のタイプ三角形
			graphicsPipelineStateDesc.PrimitiveTopologyType =
				D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		
			//どのように画面に色を打ち込むのか設定
			graphicsPipelineStateDesc.SampleDesc.Count = 1;
			graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
			
			hr = directXCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
				IID_PPV_ARGS(&graphicsPipelineState));
			assert(SUCCEEDED(hr));
		
		#pragma endregion
			
			
		

}










void SpriteCommon::Draw()
{


}





void SpriteCommon::Releases()
{
	
	graphicsPipelineState->Release();
	signatureBlob->Release();
	if (errorBlob) {
		errorBlob->Release();
	}
	rootSignature->Release();
	pixelShaderBlob->Release();

	vertexShaderBlob->Release();
	
}





IDxcBlob* SpriteCommon::CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler)
{
	






#pragma region 1.hlslファイルを読む
		//1.hlslファイルを読む
		Log(ConvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;
#pragma endregion





#pragma region 2.Compileする

	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E",L"main",
		L"-T",profile,
		L"-Zi",L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};

	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(&shaderSourceBuffer, arguments, _countof(arguments), includeHandler, IID_PPV_ARGS(&shaderResult));
	//コンパイルエラーではなくdxcが起動出来ないなど致命的な状況
	assert(SUCCEEDED(hr));

#pragma endregion




#pragma region 3.警告・エラーが出ていないかを確認する

	//警告・エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		assert(false);
	}
#pragma endregion


#pragma region 4.Compile結果を受け取って返す
	//4.Compile結果を受け取って返す
	//BLOB・・・BinaryLargeOBject
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));

#pragma endregion
	//成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却
	return shaderBlob;

#pragma endregion

}



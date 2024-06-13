#include "SpriteCommon.h"
#include <cassert>
#include"Base.h"
#include"externals/DirectXTex/d3dx12.h"
#include<vector>
#include <stdexcept>
#include <d3d12.h>  

#include <comdef.h> // For _com_error
#include <iostream>
#include <filesystem>


void SpriteCommon::Initialize(DirectXCommon* directXCommon)
{
	this->directXCommon = directXCommon;
	IDxcBlob* CompileShader(
		const std::wstring & filePath,
		const wchar_t* profile,
		IDxcUtils * dxcUtils,
		IDxcCompiler3 * dxcCompiler,
		IDxcIncludeHandler * includeHandler);
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

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	ZeroMemory(&rootSignatureDesc, sizeof(rootSignatureDesc));

		//RootSignature・・ShaderとResourceをどのように間レンズけるかを示したオブジェクトである
		;
		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		
		//SRV
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0;
		descriptorRange[0].NumDescriptors = 1;
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


		D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
		descriptorRangeForInstancing[0].BaseShaderRegister = 0;
		descriptorRangeForInstancing[0].NumDescriptors = 1;
		descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;



		//Material設定
		D3D12_ROOT_PARAMETER rootParameters[4] = {};
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[0].Descriptor.ShaderRegister = 0;
		/*
	    rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		rootParameters[1].Descriptor.ShaderRegister = 0;*/
	
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
		rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);

	
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
		



		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[3].Descriptor.ShaderRegister = 1;



		
	

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		//サンプラー設定
		D3D12_STATIC_SAMPLER_DESC staticSamples[1] = {};
		staticSamples[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		staticSamples[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamples[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamples[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamples[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamples[0].MaxLOD = D3D12_FLOAT32_MAX;
		staticSamples[0].ShaderRegister = 0;
		staticSamples[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		descriptionRootSignature.pStaticSamplers = staticSamples;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamples);



		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
		if (FAILED(hr)) {
			Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
			assert(false);
		}
		
		hr = directXCommon->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
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
		/* vertexShaderBlob = CompileShader(L"Object3D.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
		assert(vertexShaderBlob != nullptr);
	
		 pixelShaderBlob = CompileShader(L"Object3D.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
		assert(pixelShaderBlob != nullptr);*/

		vertexShaderBlob = CompileShader(L"Particle.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
		assert(vertexShaderBlob != nullptr);

		pixelShaderBlob = CompileShader(L"Particle.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
		assert(pixelShaderBlob != nullptr);
	#pragma endregion


		#pragma region PSOを生成
			////PSO生成
			D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
			graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();
			graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
			graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };
			vertexShaderBlob->GetBufferSize();
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
			
			//DepthstencStatの設定
			D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
			//Depthの機能の有効かする
			depthStencilDesc.DepthEnable = true;

			//書き込みします
			depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

			//比較関数はLessEqual.つまり近ければ描画
			depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

			//DepthStencilの設定
			graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
			graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

			hr = directXCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
				IID_PPV_ARGS(&graphicsPipelineState));
			assert(SUCCEEDED(hr));
		

		#pragma endregion
			
			
		

}







void SpriteCommon::Releases()
{
	//intermediateResource->Release();
	graphicsPipelineState->Release();
	signatureBlob->Release();
	if (errorBlob) {
		errorBlob->Release();
	}
	rootSignature->Release();
	pixelShaderBlob->Release();

	vertexShaderBlob->Release();
	intermediateResource_->Release();
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









ID3D12Resource* SpriteCommon::CreateBufferResource(ID3D12Device* device, size_t sizeInByte)
{
	
		ID3D12Resource* resource = nullptr;
		////VertexResourceを生成
		//頂点リソース用のヒープを設定
		D3D12_HEAP_PROPERTIES uploadHeapProperties{};
		uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		//頂点リソースの設定
		D3D12_RESOURCE_DESC vertexResourceDesc{};
		//バッファリソース。テクスチャの場合はまた別の設定をする
		vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vertexResourceDesc.Width = sizeInByte;
		//バッファの場合はこれらは1にする決まり
		vertexResourceDesc.Height = 1;
		vertexResourceDesc.DepthOrArraySize = 1;
		vertexResourceDesc.MipLevels = 1;
		vertexResourceDesc.SampleDesc.Count = 1;
		//バッファの場合はこれにする決まり
		vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//実際に頂点リソースを作る

		HRESULT	hr = device->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&vertexResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&resource));
		assert(SUCCEEDED(hr));


		return  resource;
	
}









ID3D12Resource* SpriteCommon::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metaData)
{
	
		//1.metadataを基にResourceの設定
		D3D12_RESOURCE_DESC resourceDesc{};
		//Textureの幅
		resourceDesc.Width = UINT(metaData.width);
		//Textureの高さ
		resourceDesc.Height = UINT(metaData.height);
		//mipmapの数
		resourceDesc.MipLevels = UINT16(metaData.mipLevels);
		//奥行き or 配列Textureの配列数
		resourceDesc.DepthOrArraySize = UINT16(metaData.arraySize);
		//TextureのFormat
		resourceDesc.Format = metaData.format;
		//サンプリングカウント
		resourceDesc.SampleDesc.Count = 1;
		//Textureの次元数。普段使っているのは2次元
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metaData.dimension);

		//2.利用するHeapの設定
		//利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース版がある
		D3D12_HEAP_PROPERTIES heapProperties{};
		//細かい設定を行う
		heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
		//WriteBackポリシーでCPUアクセス可能
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		//プロセッサの近くに配置
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;



		//3.Resourceを生成する
		ID3D12Resource* resource = nullptr;
		HRESULT	hr = device->CreateCommittedResource(
			&heapProperties,					//Heapの設定
			D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定
			&resourceDesc,						//Resourceの設定
			D3D12_RESOURCE_STATE_COPY_DEST,	//初回のResourceState。Textureは基本読むだけ
			nullptr,							//Clear最適値。使わないのでnullptr
			IID_PPV_ARGS(&resource));			//作成するResourceポインタへのポインタ
		assert(SUCCEEDED(hr));

		return resource;

	
}














DirectX::ScratchImage SpriteCommon::LoadTexture(const std::wstring& filePath)
{
	HRESULT hr{};
	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	hr = DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	
	assert(SUCCEEDED(hr));

	//ミップマップの作成
	//ミップマップ...元画像より小さなテクスチャ群
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(
		image.GetImages(), image.GetImageCount(), image.GetMetadata(),
		DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//ミップマップのデータを返す
	return image;
	
	

}


[[nodiscard]]
ID3D12Resource* SpriteCommon::UploadTewtureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(directXCommon->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	

	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	intermediateResource_ = CreateBufferResource(directXCommon->GetDevice(), intermediateSize);
	////
	////// コピー前にリソースの状態をD3D12_RESOURCE_STATE_COPY_DESTに遷移
	////D3D12_RESOURCE_BARRIER barrierToCopyDest = {};
	////barrierToCopyDest.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	////barrierToCopyDest.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	////barrierToCopyDest.Transition.pResource = texture;
	////barrierToCopyDest.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	////barrierToCopyDest.Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
	////barrierToCopyDest.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	////directXCommon->GetCommandList()->ResourceBarrier(1, &barrierToCopyDest);


	UpdateSubresources(directXCommon->GetCommandList(), texture, intermediateResource_, 0, 0, UINT(subresources.size()), subresources.data());

	//Tetureへの転送後には利用するできるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	directXCommon->GetCommandList()->ResourceBarrier(1, &barrier);
	OutputDebugStringA("Hello,DirectX!\n");
	return intermediateResource_;

}







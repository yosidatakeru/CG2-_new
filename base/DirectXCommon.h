#pragma once
#include"MyFunction.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include"WinApp.h"
#include <string>
#include <format>
#include <dxcapi.h>


#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")


class DirectXCommon
{
public:
	//初期化
	void Initialize(WinApp*winApp);

	//更新
	void Update();



	void PreDraw();

	void PosDeaw();
	

	void Releases();

     ID3D12Device* GetDevice() const { return device; }


	 ID3D12GraphicsCommandList* GetCommandList() const { return commandList; }
	 //IDxcUtils* GetDxcUtils() const { return dxcUtils; }

	 D3D12_VIEWPORT* GetViewport(){ return &viewport; }
private:
	//デバイスの初期化
	void InitializeDevice();
	//コマンドの初期化
	void InitializeCommand();

	//スワップチェーン
	void InitializeSwapchain();

	//レンダーターゲット
	void InitializeRenderTargetView();

	//深度バッファ
	void InitializeDepthBuffer();

	//フェンス
	void InitializeFence();

	ID3D12DescriptorHeap* CreateDescriptorHeap
	(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

private:
	HRESULT hr;
	WinApp* winApp = nullptr;

	ID3D12Device* device = nullptr;
	
	IDXGIFactory7* dxgiFactory = nullptr;
	
	ID3D12CommandQueue* commandQueue = nullptr;

	IDXGISwapChain4* swapChain = nullptr;

	ID3D12GraphicsCommandList* commandList = nullptr;

	ID3D12Resource* swapChainResources[2] = { nullptr };
	
	
	
	uint64_t fenceValue = 0;

	ID3D12Fence* fence = nullptr;
	
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	
	ID3D12CommandAllocator* commandAllocator = nullptr;

	ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;

	ID3D12DescriptorHeap* srvDescriptorHeap = nullptr;

	IDXGIAdapter4* useAdapter = nullptr;

	ID3D12Debug1* debugController = nullptr;

	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	HANDLE fenceEvent;

	UINT backBufferIndex;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle;

	//RTVを２つ作るのでディスクリプタを２つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];

#pragma region DescriptorHandleとDescriptorHeap
	////DescriptorHandleとDescriptorHeap
	typedef struct D3D12_CPU_DESCRIPTOR_HANDLE
	{
		SIZE_T ptr;
	}D3D12_CPU_DESCRIPTOR_HANDLE;
#pragma endregion

	D3D12_RESOURCE_BARRIER barrier{};

	
	//ビューポート
	D3D12_VIEWPORT viewport{};
};


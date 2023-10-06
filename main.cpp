#include<Windows.h>
#include<string>
#include<format>

#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
 
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")






#pragma region ウィンドウプロシージャ
//システムメッセージを処理する関数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{

	case WM_DESTROY:
	
			PostQuitMessage(0);
			return 0;

	}
	
	return DefWindowProc(hwnd, msg, wparam, lparam);

}
#pragma endregion


#pragma region Logの出力
void Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());
}

void Log(const std::wstring& message)
{
	OutputDebugStringW(message.c_str());
}

////string->wstring
std::wstring ConvertString(const std::string& str);

//wstring->string
std::string ConvertString(const std::wstring& str);
#pragma endregion











//Windowsアプリエントリ-ポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	MSG msg{};//メッセージ






#pragma region ウィンドウの生成(設定)
	//ウィンドウサイズ
	const int32_t kClienWidth = 1280;//横
	const int32_t kClientHeight = 720;//縦
	//ウィンドウクラスの設定
	WNDCLASSEX  w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;//ウインドウプロシージャを設定
	w.lpszClassName = L"DIrectXGame";  //ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);//ウィンドハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル設定

	//ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);

	//ウィンドウサイズ{X座標, Y座標　横幅, 縦幅 }
	RECT wrc = { 0,0,kClienWidth , kClientHeight};

	//自動でサイズを修正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);




#pragma endregion





#pragma region ウィンドウオブジェクトの生成
	HWND hwnd = CreateWindow(
		w.lpszClassName,//クラス名
		L"DirectXGame",//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,//標準的なウィンドウスタイル
		CW_USEDEFAULT,//表示X座標（OSに任せる）
		CW_USEDEFAULT,//表示Y座標（OSに任せる）
		wrc.right - wrc.left,//ウインド横幅
		wrc.bottom - wrc.top,//ウインドt縦幅
		nullptr,//親ウインドウハンドル
		nullptr,//メニューハンドル
		w.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//オプション

	ShowWindow(hwnd, SW_SHOW);

#pragma endregion







#pragma region DXGIファクトリーの生成

	//DXGIファクトリーの生成
	IDXGIFactory7* dxgiFactory = nullptr;

	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	assert(SUCCEEDED(hr));



#pragma endregion



#pragma region 使用するアダプタをGPUを決める

	//使用するアダプター用の変数最初にnullptrを入れておく
	IDXGIAdapter4* useAdapter = nullptr;

	//よい順でアダプタを頼む

	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; i++)
	{
		//アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));//取得できないのは一大事


		//ソフトエアアダプタ出なければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			Log(std::format(L"Use Adapater;{}\n", adapterDesc.Description));
			break;
		}

		useAdapter = nullptr;

	}
	//適切なアダプタが見つからなかったので起動出来ない
	assert(useAdapter != nullptr);



#pragma endregion




#pragma region D3D12Deviceの生成
	ID3D12Device* device = nullptr;

	//機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] =
	{

		D3D_FEATURE_LEVEL_12_2,  D3D_FEATURE_LEVEL_12_1,  D3D_FEATURE_LEVEL_12_0
	};



	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };


	//高い順に生成できるため
	for (size_t i = 0; i < _countof(featureLevels); ++i)
	{
		hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));

		if (SUCCEEDED(hr))
		{
			//生成できたのでログ出力を行ってループを抜ける
			//Log(std::format("FeatureLevel : {}\n"), featureLevelStrings[i]);

			break;


		}

	}
	assert(device != nullptr);

	Log("Complete create D3D12Device!!!\n");//初期化完了ログを出す

	
#pragma endregion




#pragma region CommandQueueを生成
	//コマンドキュを生成
	ID3D12CommandQueue* commandQueue = nullptr;

	D3D12_COMMAND_QUEUE_DESC commandQurDesc{};

	hr = device->CreateCommandQueue(&commandQurDesc,
		IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(hr));
#pragma endregion



#pragma region CommandListを生成
	//コマンドアロケータを生成
	ID3D12CommandAllocator* commandAllocator = nullptr;

	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));

	assert(SUCCEEDED(hr));


	//コマンドリストの生成
	ID3D12GraphicsCommandList* commandList = nullptr;

	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr,
		IID_PPV_ARGS(&commandList));

	assert(SUCCEEDED(hr));

#pragma endregion



#pragma region CSwapChainを生成する

//コマンドキューを使うためコマンドキュー生成後、メインループ前に行う
//スワップチェーン生成
	IDXGISwapChain4* swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = kClienWidth;//画面幅。ウィンドウクライアント領域をおマジものにしておく
	
	swapChainDesc.Height = kClientHeight;//画面の高さ。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色の形式
	swapChainDesc.SampleDesc.Count = 1;//マルチサンプル
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//描画ターゲットとして利用する
	swapChainDesc.BufferCount = 2;//ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//モニタの移したら中身の破壊

	//コマンドキューウィンドウハンドル　設定を渡して生成する
	hr = dxgiFactory->CreateSwapChainForHwnd(commandQueue, hwnd, &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain));
	assert(SUCCEEDED(hr));

#pragma endregion






#pragma region DescriptorHeapを生成する
	//ディスクリプターの生成
	
	ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー用
	rtvDescriptorHeapDesc.NumDescriptors = 2;//ダブルバッファ用に２つ。多くてもかまわない
	hr = device->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap));
	//ディスクリプタヒープが作られなかったので起動できない
	assert(SUCCEEDED(hr));













#pragma endregion


#pragma region SwapChainからResourceを引っ張てくる
	
	//swapChainから	Resourceを引っ張てくる
	ID3D12Resource* swapChainResources[2] = { nullptr };
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));

	assert(SUCCEEDED(hr));

	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[1]));

	assert(SUCCEEDED(hr));
#pragma endregion




#pragma region RTVを作る



	//RTVを作る
	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;				//出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;			//2dテクスチャとして書き込む
	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	//RTVを２つ作るのでディスクリプタを２つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2] = {};
	//まず1つ目を作る。１つ目は最初の所に作る。作る場所をこちらで指定してあげる必要がある
	rtvHandles[0] = rtvStartHandle;
	device->CreateRenderTargetView(swapChainResources[0], &rtvDesc, rtvHandles[0]);
	//２つ目のディスクリプタハンドルを得る(自力で)
	rtvHandles[1].ptr = rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//２つ目を作る
	device->CreateRenderTargetView(swapChainResources[1], &rtvDesc, rtvHandles[1]);



	//DescriptorHandleとDescriptorHeap
	typedef struct D3D12_CPU_DESCRIPTOR_HANDLE {
		SIZE_T ptr;
	}D3D12_CPU_DESCRIPTOR_HANDLE;

	////Descriptorの位置を決める
	rtvHandles[0] = rtvStartHandle;

	
	rtvHandles[1].ptr = rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	



#pragma endregion






#pragma region コマンドを積み込んで確定させる

	//これから書き込むバックバッファのインデックスを取得
		UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();
	
		//画面先のRTVを設定する
		commandList->OMSetRenderTargets(1, &rtvHandles[backBufferIndex], false, nullptr);

		//指定した色で画面全体をクリアする
		float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };//青っぽい色。RGBA順

		commandList->ClearRenderTargetView(rtvHandles[backBufferIndex], clearColor, 0, nullptr);

		//コマンドリストの内容を確定させる。すべてのコマンドを積んでからＣｌｏｓｅすること
		hr = commandList->Close();
		assert(SUCCEEDED(hr));
	

#pragma endregion




#pragma region コマンドをキックする

//GPUにコマンドリストの実行を行わせる
ID3D12CommandList* commandLists[] = { commandList };

commandQueue->ExecuteCommandLists(1, commandLists);

//GPUとOSに画面交換を行う言う通知する
swapChain->Present(1, 0);

//次のフレーム用のコマンドリストを準備
hr = commandAllocator->Release();
assert(SUCCEEDED(hr));

hr = commandList->Reset(commandAllocator, nullptr);
assert(SUCCEEDED(hr));


#pragma endregion



#pragma endregion

	//ウィンドウの固定
	//ゲームループ
	while (true)
	{
		//メッセージがある?
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);//キー入力メッセージの処理
			DispatchMessage(&msg);//プロシージャにメッセージを送る
		}
		else
		{

		}



	}
	
	//ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
	return 0;


}




#pragma region Logの出力
std::wstring ConvertString(const std::string& str)
{
	if (str.empty())
	{
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0)
	{
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

std::string ConvertString(const std::wstring& str)
{
	if (str.empty())
	{
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0)
	{
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}
#pragma endregion
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
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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
	const int window_width = 1280;//横
	const int window_height = 720;//縦
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
	RECT wrc = { 0,0,window_width,window_height };

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




#pragma endregion

	//ウィンドウの固定
	//ゲームループ
	while (true)
	{
		//メッセージがある?
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);//キー入力メッセージの処理
			DispatchMessage(&msg);//プロシージャにメッセージを送る
		}
		//xボタンで終了のメッセージが来たらゲームループを抜ける
		if (msg.message == WM_QUIT)
		{
			break;
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
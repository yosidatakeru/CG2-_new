#include"WinApp.h"

#pragma region ウィンドウプロシージャ
//Window Procedure(関数)
LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	
	switch (msg)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		{
			return true;
		}

		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
#pragma endregion


void WinApp::Initialize()
{
#pragma region ウィンドウクラスの登録6

	////ウィンドウクラスを登録する

	//ウィンドウプロシージャ
	wc.lpfnWndProc = WindowProc;
	//ウィンドウクラス名
	wc.lpszClassName = L"dada";
	//インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wc);

#pragma endregion



#pragma region ウィンドウサイズを決める

	


	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0,kClientWidth ,kClientHeight };

	//クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

#pragma endregion



#pragma region ウィンドウプロシージャ

	//ウィンドウの生成
	 hwnd = CreateWindow(
		wc.lpszClassName,		//利用するクラス名
		L"CG2",					//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	//よく見るウィンドウスタイル
		CW_USEDEFAULT,			//表示X座標
		CW_USEDEFAULT,			//表示Y座標
		wrc.right - wrc.left,	//ウィンドウ横軸
		wrc.bottom - wrc.top,	//ウィンドウ縦軸
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		wc.hInstance,			//インスタンスハンドル
		nullptr);				//オプション
	//ウィンドウを表示する
	ShowWindow(hwnd, SW_SHOW);

	
}

bool WinApp::Update()
{
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}
	//×ボタンで終了
	if (msg.message == WM_QUIT) 
	{
		return true;
	}

	return false;
}


#include<Windows.h>

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



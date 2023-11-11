#include <Windows.h>
#include <cstdint>
class WinApp
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
		WPARAM wparam, LPARAM lparam);

	//初期化
	void Initialize();

	//更新
	bool Update();

	

	

	HWND GetHwnd() const { return hwnd; }

	HINSTANCE GetHInstanc() const { return wc.hInstance; }
public:
	//クライアント領域のサイズ
	static	const int kClientWidth = 1280;
	static	const int kClientHeight = 720;


private:


	MSG msg{};
	WNDCLASS wc{};
	HWND hwnd;



};


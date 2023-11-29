#include <Windows.h>
#include <cstdint>
#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
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


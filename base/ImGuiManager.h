#pragma once
#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"
//#include"WinApp.h"
#include"DirectXCommon.h"
class ImGuiManager
{
public:
	void Initialize(WinApp* winApp_, DirectXCommon* directXcommon_);
	
	void BeginFlame(DirectXCommon* directXcommon_);


	void EndFlame(DirectXCommon* directXcommon_);

	void Release();
private:
	WinApp* winApp = nullptr;
	DirectXCommon* directXcommon = nullptr;
};


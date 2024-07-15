#pragma once

#include<Windows.h>
#define DIRECTINPUT_VERSION     0x0800  // DirectInputのバージョン指定
#include <dinput.h>
#include<wrl.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include"DirectXCommon.h"

class Input
{
public:



	void Initialize(WinApp* winApp);

	void Update();

	bool PushKey(BYTE keyNumber);

	bool TriggerKey(BYTE keyNumber);

private:
	WinApp* winApp_ = nullptr;

	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;

	BYTE key[256] = {};

	BYTE keyPre[256] = {};
};


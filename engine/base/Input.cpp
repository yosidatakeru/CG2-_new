#include "Input.h"
#include <cassert>
#include<iostream>

void Input::Initialize(WinApp* winApp)
{
    winApp_ = winApp;
    HRESULT result;

    // DirectInputの初期化
    Microsoft::WRL::ComPtr<IDirectInput8> directInput;
    result = DirectInput8Create(
        winApp_->GetHInstanc(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
    assert(SUCCEEDED(result));


    // キーボードデバイスの生成

    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    // 入力データ形式のセット
    result = keyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
    assert(SUCCEEDED(result));
    // 排他制御レベルのセット
    result = keyboard->SetCooperativeLevel(
        winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));
}

void Input::Update()
{
    memcpy(keyPre, key, sizeof(key));

    // キーボード情報の取得開始
    keyboard->Acquire();
    // 全キーの入力状態を取得する
    keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber)
{
    if (key[keyNumber])
    {
        return true;
    }

    return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
    if (keyPre[keyNumber] == false && key[keyNumber])
    {
        return true;
    }

    return false;
}

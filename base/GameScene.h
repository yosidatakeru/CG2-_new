#pragma once
#include"Base.h"
#include"DirectXCommon.h"
#include"SpriteCommon.h"

class GameScene
{
public:
	void Initalize();



private:
	// WinAppの生成
	WinApp* winApp = nullptr;
	// DirectXCommonの生成
	DirectXCommon* directXCommon = nullptr;



};


#include "GameScene.h"

void GameScene::Initalize()
{
#pragma region WinApp初期化
	
	winApp = new WinApp();
	winApp->Initialize();

#pragma endregion
	


#pragma region DirectX初期化
	
	directXCommon = new DirectXCommon();
	
	directXCommon->Initialize(winApp);
#pragma endregion
}

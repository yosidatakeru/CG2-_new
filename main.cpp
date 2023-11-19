//#include"base/WinApp.h"
#include"base/Base.h"
#include"base/DirectXCommon.h"
#include"base/SpriteCommon.h"



#pragma endregion

//CompilerShader関数






//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");
#pragma region ポインタ置き場
	WinApp* winApp = nullptr;
	DirectXCommon* directXCommon = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	
#pragma endregion

#pragma region WinApp初期化
	winApp = new WinApp();
	winApp->Initialize();
#pragma endregion





#pragma region DirectX初期化

	directXCommon = new DirectXCommon();
	directXCommon->Initialize(winApp);
#pragma endregion









	

#pragma region	三角形の描画
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(directXCommon);
#pragma endregion
	
	


	Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	////メインループ
	//ウィンドウの✕ボタンが押されるまでループ
	while (true)
	{
		
		//Windowにメッセージが来てたら最優先で処理させる
		if (winApp->Update()==true)
		{
			break;
		}
		else {
			//ゲームの処理

			

		}
		
		directXCommon->PreDraw();
		transform.rotate.y += 0.03f;
		spriteCommon->Draw();
		
		spriteCommon->Update(transform);
		
		
		directXCommon->PosDeaw();
	}



#pragma region 解放処理
	
	spriteCommon->Releases();

	directXCommon->Releases();

	CloseWindow(winApp->GetHwnd());

#pragma endregion



#pragma region ReportLiveObjects
	
	////リソースリークチェック
	IDXGIDebug1* debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}

#pragma endregion

#pragma region delete
	delete winApp;
	delete directXCommon;
	delete spriteCommon;
#pragma endregion
	return 0;
}





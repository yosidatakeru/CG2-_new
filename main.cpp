//#include"base/WinApp.h"
#include"base/Base.h"
#include"base/DirectXCommon.h"
#include"base/SpriteCommon.h"
#include"base/ImGuiManager.h"
#include"Sprite.h"



#pragma endregion

//CompilerShader関数






//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//COMの初期化

	CoInitializeEx(0, COINIT_MULTITHREADED);
	


#pragma region ポインタ置き場
	WinApp* winApp = nullptr;
	DirectXCommon* directXCommon = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	ImGuiManager* imGuiManager = nullptr;
	Sprite* sprite = nullptr;
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
	sprite = new Sprite();
	sprite->Initialize(directXCommon, spriteCommon);
#pragma endregion



#pragma region ImGuiの初期化
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApp,directXCommon);
#pragma endregion
	
	
	Matrix4x4* camera = nullptr;


	Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
	

	sprite->GetwvpResource()->Map(0, nullptr, reinterpret_cast<void**>(&camera));


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
		//transform.rotate.y += 0.08f;
		directXCommon->PreDraw();
		imGuiManager->BeginFlame(directXCommon);
		sprite->Draw(transform, cameraTransform);
		
		
		
		
		
				
		imGuiManager->EndFlame(directXCommon);
		
		directXCommon->PosDeaw();

		
	}



#pragma region 解放処理
	
	sprite->Releases();
	spriteCommon->Releases();
	
	directXCommon->Releases();

	imGuiManager->Release();

	CloseWindow(winApp->GetHwnd());


#pragma endregion



#pragma region ReportLiveObjects
	
	////リソースリークチェック
	IDXGIDebug1* debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) 
	{
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


	//COMの終了処理
	CoUninitialize();
	return 0;
}





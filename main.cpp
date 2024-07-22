#include"engine/base/Input.h"
#include"engine/mata/Base.h"
#include"engine/base/DirectXCommon.h"
#include"engine/2d/SpriteCommon.h"
#include"engine/base/ImGuiManager.h"
#include"engine/2d/Sprite.h"
#include"base/ResourceObject.h"
#include"engine/mata/TextureManager.h"
#include"engine/3d/Object3dCommon.h"
#include"engine/3d/Object3d.h"
#include"engine/3d/Model.h"
#include"engine/3d/ModelManager.h"
#include"engine/base/Camera.h"

//#include <engine/3d/ModelManager.h>
#pragma endregion

//CompilerShader関数






//Winodwsアプリでもエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//COMの初期化

	CoInitializeEx(0, COINIT_MULTITHREADED);
	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");


#pragma region ポインタ置き場
	WinApp* winApp = nullptr;
	DirectXCommon* directXCommon = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Object3dCommon* object3dCommon = nullptr;
	Object3d* object3d = nullptr;
	Input* input = nullptr;
	ImGuiManager* imGuiManager = nullptr;
	ModelCommon* modelCommon = nullptr;
	Model* model = nullptr;
	
	
#pragma endregion
	//スプライト描画数
	int drawSprit = 5;
	//モデル描画数
	int drawmodel = 5;

	
	
	

#pragma region WinApp初期化
	winApp = new WinApp();
	winApp->Initialize();
#pragma endregion




#pragma region DirectX初期化

	directXCommon = new DirectXCommon();
	directXCommon->Initialize(winApp);
#pragma endregion

	input = new Input();
	input->Initialize(winApp);

	
	//テクスチャマネージャー
	TextureManager::GetInstance()->Initialize(directXCommon);


	
	

#pragma region	三角形の描画
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(directXCommon);

	
	
	TextureManager::GetInstance()->LoadTexture(L"Resources/Player5.png");
	TextureManager::GetInstance()->LoadTexture(L"Resources/uvChecker.png");
	std::vector<Sprite*> sprites;
	for (int i = 0; i < drawSprit; i++)
	{
		Sprite* sprite = new Sprite();
		if (i == 4)
		{
			sprite->Initialize(directXCommon, spriteCommon, L"Resources/Player5.png");
		}
		else 
		{
			sprite->Initialize(directXCommon, spriteCommon, L"Resources/Player5.png");
		}
		sprite->SetPosintion({(float)i * 120,0});
		sprites.push_back(sprite);
	}


#pragma endregion


#pragma region	3Dオブジェクト(3Dモデル)

	ModelManager::GetInstance()->Initslize(directXCommon);
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	std::vector<Model*> models;
	std::vector<Object3d*> object3ds;

	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(directXCommon);


	////カメラ
	
	
	

	modelCommon = new ModelCommon();
	modelCommon->Initialze(directXCommon);


	
	for (int i = 0; i < drawmodel; i++)
	{

		
		model = new Model();
		object3d = new Object3d();


		if (i == 4)
		{
			model->Initialize(modelCommon, "Resources", "axis.obj");
			object3d->Initialize(object3dCommon);
			object3d->SetModels("axis.obj");
			object3d->SetModel(model);
		}
		else
		{

			model->Initialize(modelCommon, "Resources", "plane.obj");
			object3d->Initialize(object3dCommon);
			object3d->SetModels("plane.obj");
			object3d->SetModel(model);
		}

		model->SetPosintion({ (float)i * 2.0f });
		models.push_back(model);
		object3ds.push_back(object3d);

	}
		
	



#pragma endregion



#pragma region ImGuiの初期化
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApp,directXCommon);
#pragma endregion
	
	
	
	


	 //sprites[1]->GetwvpResource()->Map(0, nullptr, reinterpret_cast<void**>(&camera));


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

		// カメラの更新
		

		imGuiManager->BeginFlame(directXCommon);
	  
		input->Update();

		//camera->Update(input);
	//スプライトの描画
	
	//いろいろな処理(色、回転、移動など)	
	//移動処理
	/* Vector2 pos = sprites[1]->GetPosition();
	 pos.x += 0.5f;
	 sprites[1]->SetPosintion(pos);*/
     //回転の処理
	 //Vector3 rot = sprites[1]->GetRotation();
	 //rot.y += 0.5f;
	 //sprites[1]->SetRotaion(rot);
	 ////
		
	//色の処理
	 /*Vector4 color = sprite->GetColor();
	 color.x = 1.0f;
	 sprite->SetColor(color);*/

	  //Vector2 size = sprite->GetSize();
      //size.x += 0.1f;
	  //size.y += 0.1f;
	  //sprite->SetSize(size);
		
	  
		if (input->PushKey(DIK_A))
		{
			Vector3 pos = models[1]->GetPosition();
			pos.x = -10.0f;
			models[1]->SetPosintion(pos);
		}

	 /* rot = models[3]->GetRotation();
	  rot += 0.05f;
	  models[3]->SetRotaion(rot);*/
	
	 /*Vector3 pos = models[1]->GetPosition();
	 pos.z += 0.5f;
	 models[1]->SetPosintion(pos);

		*/
		
		Vector3 pos = object3d->GetCameraPosition();
		pos.x = 0.0f;
		object3d->SetCameraPosintion(pos);
		
		Vector3 rot = object3d->GetCameraRotation();

	 for (int i = 0; i < drawmodel; i++)
	 {

		 
		 object3ds[i]->CameraUpdate(pos,rot);
		 object3ds[i]->Update(models[i]->GetTransform());
		
		 
	 }
	
	

	 for (int i = 0; i < drawSprit; i++)
	 {
	 	sprites[i]->Update(sprites[i]->GetTransform(), sprites[i]->GetCameraTransform(), sprites[i]->GetTransformSprite());
	 }
	    
	 directXCommon->PreDraw();
	     
	 
	
	    
	 
	 //オブジェクトの描画
	 for (int i = 0; i < drawmodel; i++)
	 {
	 	object3dCommon->Object3dPreDraw();
	 	object3ds[i]->Draw();
	 }
	    
	    
	 
	    
	 for (int i = 0; i < drawSprit; i++)
	 {
	 	spriteCommon->SpritePreDraw();
	 	sprites[1]->Draw(directXCommon);
	 	
	 }
	 
	 
	    
	    
	    
	    
	    
	 imGuiManager->EndFlame(directXCommon);
	 
	 directXCommon->PosDeaw();
	    
	 
	}



#pragma region 解放処理

	for (int i = 0; i < drawmodel; i++)
	{
		object3ds[i]->Releases();
	}

	for (int i = 0; i < drawSprit; i++)
	{
		sprites[i]->Releases();
	}
	
	object3dCommon->Releases();

	spriteCommon->Releases();

	directXCommon->Releases();

	imGuiManager->Release();

	CloseWindow(winApp->GetHwnd());


#pragma endregion



#pragma region ReportLiveObjects

	////リソースリークチェック
	//こいつを最後に呼び出す
	//struct D3DResourceLeakChecker
	struct D3DResourceLeakChecker
	{
		~D3DResourceLeakChecker()
		{

			IDXGIDebug1* debug;
			if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
			{
				debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
				debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
				debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
				debug->Release();
			}
		}
	};

	
	delete object3d;
	
	ModelManager::GetInstance()->Finalize();

	for (int i = 0; i < 5; i++)
	{
		delete sprites[i];
	}
	 TextureManager::GetInstance()->Finalize();
#pragma endregion

#pragma region delete
	 delete input;
	delete winApp;
	delete directXCommon;
	//delete spriteCommon;
#pragma endregion


	//COMの終了処理
	CoUninitialize();
	return 0;
}





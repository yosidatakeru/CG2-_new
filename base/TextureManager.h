#pragma once
#include <string>
#include "externals/DirectXTex/DirectXTex.h"
#include <wrl.h>
#include <d3d12.h>
#include "DirectXCommon.h"

#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "d3d12.lib")
class TextureManager
{
private:
	//テクスチャデータの構造体
    //テクスチャ1枚分のデータ
	struct TextureData
	{
		std::string filePath;                            //画像ファイルパス
		DirectX::TexMetadata metadata;                  //画像の幅や高さなどの情報
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;//テクスチャリソーソ
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;      //SRV作成時に必要なCPUハンドル
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;      //描画コマンドに必要なGPUハンドル
	}; 
	
public:

	//初期化
	void Initialize();

	//シングルトンインスタンスの取得
	static TextureManager* GetInstance();
	//終了
	void Finalize();
private:
	static TextureManager* instance;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager& operator=(TextureManager&) = delete;
	//テクスチャデータの配列
	//普通の配列でわなく読み込みテクスチャが何枚あるかカウントもちろん読み込む前は0
	std::vector<TextureData> textureData;

};


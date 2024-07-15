#include"DirectXCommon.h"
#include<string>

#include"engine/base/externals/DirectXTex/DirectXTex.h"

class TextureManager
{
private:
	//画像描画するために必要のデータ
	struct TexTureData
	{
		std::wstring filePath;
		DirectX::TexMetadata metaData;
		Microsoft::WRL::ComPtr<ID3D12Resource>resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};


public:
	static TextureManager* GetInstance();
	void Finalize();

	void Initialize(DirectXCommon* directXCommon);
	//画像の読み込み
	void  LoadTexture(const std::wstring& filePath);

	//指定した画像が配列の何番目にあるか確認
	uint32_t GetTextureIndexFilePath(const std::wstring& filePath);

	//指定したが要素番号ハンドルを受け取る
	D3D12_GPU_DESCRIPTOR_HANDLE GetStvHandleGPU(uint32_t textureIndex);

	//メタデータ取得
	const DirectX::TexMetadata& GetMetaData(uint32_t textureIndex);
private:
	//シェーダーに送る
	void UploadTewtureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);
private:
	static TextureManager* instance;
	static uint32_t kSRVIndexTop;


	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInByte);
	
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metaData);
	
private:
	std::vector<TexTureData> textureDatas;
	DirectXCommon* directXCommon_ = nullptr;
};

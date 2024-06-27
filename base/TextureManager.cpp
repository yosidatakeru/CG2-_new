#include "TextureManager.h"


TextureManager* TextureManager::instance = nullptr;


void TextureManager::Initialize()
{
	//欠点として要素絵を超えると再び割り当てが発生する
	//対策として最初からメモリよ最大数確保する
	textureData.reserve(DirectXCommon::kMaxSRVCount);
}

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new TextureManager;
	}

	return instance;
}

void TextureManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

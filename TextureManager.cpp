#include "TextureManager.h"

void TextureManager::Initialize(DirectXCommon* directXCommon)
{
	directXCommon_ = directXCommon;
	textureDatas.reserve(DirectXCommon::kmixSRVcount);
}

#pragma once
#include"base/DirectXCommon.h"
#include"base/SpriteCommon.h"
class Sprite2d
{
public:
	void Initialize(DirectXCommon* directXCommon, SpriteCommon* SpriteCommon_);

	void Draw(DirectXCommon* directXCommon);
	void Update(Transform transform, Transform cameraTransform, Transform trnsformSprite);
};


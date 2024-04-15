#pragma once
#include"base/DirectXCommon.h"
#include"base/Matrix4x4.h"
#include"base/Vector4.h"
class Triangle
{
public:
	void Initialize(DirectXCommon* directXSetup);

	void Draw(Vector4 left, Vector4 top, Vector4 right, Transform transform, Matrix4x4 viewMatrix, Matrix4x4 projectionMatrix, Vector4 color);

};


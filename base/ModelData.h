#pragma once
#include"externals/DirectXTex/d3dx12.h"
#include"VertexData.h"
#include "MaterialData.h"
struct  ModelData
{
	std::vector<VertexData> vertices;
	MatrialData material;
};
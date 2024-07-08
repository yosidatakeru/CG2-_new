#pragma once
#include"ModelData.h"
#include <cassert>
#include"ModelCommon.h"
#include"VertexData.h"
#include"Material.h"
#include<fstream>
#include<sstream>


class Model
{
public:
	void Initialize(ModelCommon* modelCommon);

	void Draw();

	

	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	MatrialData LoadMatrialTemplateFile(const std::string& directoryPath, const std::string& filename);


private:
	//頂点データの作成関数
	void CreateVertex();

	void CreateMAterial();


	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInByte);


public:
	//色
	Vector4 GetColor() { return color_; }
	void SetColor(Vector4 color) { color_ = color; }

	ModelData GetModelData() { return modelData; }

	

private:
	//色パラメーター
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };



private:
	
	ModelCommon* modelCommon_ = nullptr; 
	Model* model_ = nullptr;


	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	VertexData* vertexData = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource>  materialResource = nullptr;
	Material* materialData = nullptr;

	

	ModelData modelData; // 構築するMaterualData



};


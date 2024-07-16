#pragma once
#include"engine/mata/ModelData.h"
#include <cassert>
#include"ModelCommon.h"
#include"engine/mata/VertexData.h"
#include"engine/mata/Material.h"
#include<fstream>
#include<sstream>


class Model
{
public:
	void Initialize(ModelCommon* modelCommon, const std::string& directorypath, const std::string& filename);
	void Load(const std::string& filePath);

	
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

	//回転
	Vector3 GetRotation() { return rotation; }
	void SetRotaion(Vector3 rot) { rotation = rot; }

	//移動
	Vector3 GetPosition() { return position; }
	void SetPosintion(Vector3 pos) { position = pos; }

	Transform GetTransform() { return  transform_; }
	

private:
	//色パラメーター
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };


	Vector3 rotation;
	Vector3 position = { 0, 0, 0};

	Transform transform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };




private:
	
	ModelCommon* modelCommon_; 
	Model* model_ = nullptr;


	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	VertexData* vertexData = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource>  materialResource = nullptr;
	Material* materialData = nullptr;

	

	ModelData modelData; // 構築するMaterualData



};


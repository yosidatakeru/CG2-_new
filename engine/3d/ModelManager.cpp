#include "ModelManager.h"
ModelManager* ModelManager::instance = nullptr;

ModelManager* ModelManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ModelManager;
	}

	return instance;
}

void ModelManager::Initslize(DirectXCommon* directXCommon)
{
	 
	modelCommon = new ModelCommon();
	modelCommon->Initialze(directXCommon);
	
}

void ModelManager::LoadModel(const std::string& filePath)
{
	if (models.contains(filePath))
	{
		//早期リターン
		return;

		
	}

	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->Initialize(modelCommon, "Resources", filePath);

	//モデルをmapコンテナに格納
	models.insert(std::make_pair(filePath, std::move(model)));

}

Model* ModelManager::FindModel(const std::string& filePath)
{
	if (models.contains(filePath))
	{
		return models.at(filePath).get();
	}
	return nullptr;
}

void ModelManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

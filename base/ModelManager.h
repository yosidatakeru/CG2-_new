#pragma once
#include <map>
#include <string>
#include <memory>
#include "Model.h"
class ModelManager
{
public:



	//シングルトンインスタンスの取得
	static ModelManager* GetInstance();

	void Initslize(DirectXCommon* directXCommon);

	//モデルファイルの読み込み
	void LoadModel(const std::string& filePath);

	//モデルの検索
	Model* FindModel(const std::string& firlePath);

	//終了
	void Finalize();


	
private:

	static ModelManager* instance;

	//コンストラクタ
	ModelManager() = default;
	//デストラクタ
	~ModelManager() = default;
    //コピーコンストラクタ
	ModelManager(const ModelManager&) = delete;
	//代入演算子
	ModelManager& operator=(const ModelManager&) = delete;

	ModelCommon* modelCommon = nullptr;
	
	//モデルデータ
	std::map<std::string, std::unique_ptr<Model>>models;
    
	
};


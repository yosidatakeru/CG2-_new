#include "Model.h"
#include"TextureManager.h"
#include "Base.h"
#include<cassert>


void Model::Initialize(ModelCommon* modelCommon)
{
	modelCommon_ = modelCommon;	
	
	modelData = LoadObjFile("Resources", "plane.obj");

	CreateVertex();

	CreateMAterial();

	TextureManager::GetInstance()->LoadTexture(ConvertString(modelData.material.textureFilePath));

	modelData.material.textureIndex =
		TextureManager::GetInstance()->GetTextureIndexFilePath(ConvertString(modelData.material.textureFilePath));


	
}

void Model::Load(const std::string& filePath)
{
	

	
}

void Model::Draw()
{
	
	materialData->color = color_;

	modelCommon_->GetDirectXCmmon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	//マテリアルCBufferの場所を設定
	modelCommon_->GetDirectXCmmon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());


	modelCommon_->GetDirectXCmmon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetStvHandleGPU(modelData.material.textureIndex));//ライト用

	//描画(DrawCall)３兆点で１つのインスタンス。
	modelCommon_->GetDirectXCmmon()->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);


}

void Model::CreateVertex()
{
	//VertexBufferViewを作成
	//頂点バッファビューを作成する
	vertexResource = CreateBufferResource(modelCommon_->GetDirectXCmmon()->GetDevice(), sizeof(VertexData) * modelData.vertices.size());

	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	//１頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	////Resourceにデータを書き込む
	//
	////書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());

}

void Model::CreateMAterial()
{
	//Resourceにデータを書き込む
	materialResource = CreateBufferResource(modelCommon_->GetDirectXCmmon()->GetDevice(), sizeof(Material)); ;


	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color_;
	materialData->enableLighting = false;
	materialData->uvTrasform = MakeIdentity4x4();
}

ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{

	//１.中で必要となる変数の宣言
	std::vector<Vector4> positions; // 位置
	std::vector<Vector3> normals; //法線
	std::vector<Vector2> texcoords; //テクスチャ座標
	std::string line; //ファイルから読んだ1行を格納するもの

	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename); //ファイルを開く
	assert(file.is_open()); //とりあえず開けなかったら止める

	//3.実際にファイルを読み,Materiaを構築する
	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; //先頭の識別子を読む

		//identifierに応じた処理
		if (identifier == "v")
		{
			Vector4 position;

			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt")
		{
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoords.push_back(texcoord);

		}
		else if (identifier == "vn")
		{
			Vector3 normal;

			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (identifier == "f")
		{
			VertexData triangle[3];
			//面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex)
			{
				std::string vertexDefinition;
				s >> vertexDefinition;

				//頂点の要素へのIndexは「位置/UV/法線」で格納されているので分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element)
				{
					std::string index;
					std::getline(v, index, '/');//区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}

				//要素へのIndexから,実際の要素の値を取得して,頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];



				//VertexData vertex = { position, texcoord, normal };
				//modelData.vertices.push_back(vertex);


				texcoord.y *= -1.0f;
				triangle[faceVertex] = { position, texcoord, normal };
				position.x *= -1.0f;

				normal.x *= -1.0f;

			}

			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib")
		{
			//materialTemplateLidraryファイルの名前を取得
			std::string materialFilename;
			s >> materialFilename;
			//基本的にobjファイルと同一階層にmtlは存在させるの,ディレクトリ名とファイル名を渡す
			modelData.material = LoadMatrialTemplateFile(directoryPath, materialFilename);

		}


	}

	//4.ModelDataを返す

	return modelData;
}

MatrialData Model::LoadMatrialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	//1.中で必要となる変数の宣言
	MatrialData matrialData;//構築するマテリアルデータ
	std::string line;//ファイルから読んだ1行目を格納するもの

	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);//ファイルを開く
	assert(file.is_open());//とりあえず開かなかったら止める

	//3.実際にファイルを読み込み,NaterialDataを構築する
	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		//identifierに応じて処理
		if (identifier == "map_Kd")
		{
			std::string textureFilename;
			s >> textureFilename;
			//連結してファイルパスにする
			matrialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	//4.MaterialDataを返す
	return matrialData;
}

ID3D12Resource* Model::CreateBufferResource(ID3D12Device* device, size_t sizeInByte)
{
	ID3D12Resource* resource = nullptr;
	////VertexResourceを生成
	//頂点リソース用のヒープを設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInByte;
	//バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際に頂点リソースを作る

	HRESULT	hr = device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));


	return  resource;
}

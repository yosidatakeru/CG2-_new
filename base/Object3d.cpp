#include "Object3d.h"
#include "Base.h"
#include<cassert>
#include"Object3dCommon.h"
#include"TextureManager.h"

void Object3d::Initialize(DirectXCommon* directXCommon, Object3dCommon* Object3dCommon)
{
	directXCommon_ = directXCommon;
    object3dCommon_ = Object3dCommon;

    modelData = LoadObjFile("Resources", "plane.obj");

	

	CreateVertex();

	TextureManager::GetInstance()->LoadTexture(ConvertString(modelData.material.textureFilePath));

	modelData.material.textureIndex =
		TextureManager::GetInstance()->GetTextureIndexFilePath(ConvertString(modelData.material.textureFilePath));

	CreateMAterial();

	CreateWVP();


	//CreateTransform();

	//CreatLight();*/

}

void Object3d::Update(Transform transform, Transform cameraTransform, Transform transformSprite)
{

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	wvpData->WVP = worldViewProjectionMatrix;
	wvpData->World = worldMatrix;



	ImGui::Begin("texture");
	//ImGui::DragFloat3("light", &light.x, 0.01f, -1.0f, 1.0f);


	ImGui::Begin("model");

	ImGui::DragFloat3("model", &rotation, 1.0f, -1.0f, 3.0f);

	//ImGui::DragFloat3("model", &position.x, 1.0f, -1.0f, 1000.0f);

	//ImGui::DragFloat3("model", &size.x, 1.0f, -1.0f, 1000.0f);


	ImGui::End();

	
	
	


	ImGui::End();


}



void Object3d::Draw(DirectXCommon* directXCommon)
{

	transform_.translate = { 0,position.y, 0 };
	//回転パラメータ
	transform_.rotate = { 0,rotation,0 };

	materialData->color = color_;


	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	
	directXCommon_ = directXCommon;
	
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	
	

	//マテリアルCBufferの場所を設定
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	//wvp用のCBufferの場所を設定
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());



	
	directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2,  TextureManager::GetInstance()->GetStvHandleGPU(modelData.material.textureIndex));
	//ライト用
	//directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLighlResource->GetGPUVirtualAddress());



	//描画(DrawCall)３兆点で１つのインスタンス。
	directXCommon_->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);




}

void Object3d::Releases()
{
	vertexResource->Release();
	
	materialResource->Release();
	
	wvpResource->Release();
	
	//textureResource->Release();
}



void Object3d::CreateVertex()
{
	//VertexBufferViewを作成
	//頂点バッファビューを作成する
	vertexResource = object3dCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(VertexData) * modelData.vertices.size());

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






void Object3d::CreatTexture(std::wstring filePath)
{
	//////画像読み込み
	
	DirectX::ScratchImage mipImages = object3dCommon_->LoadTexture(filePath);
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	textureResource = object3dCommon_->CreateTextureResource(directXCommon_->GetDevice(), metaData);

	//画像データを送る
	ID3D12Resource* texture = object3dCommon_->GetIntermediateResource();
	texture = object3dCommon_->UploadTewtureData(textureResource, mipImages);
	object3dCommon_->SetIntermediateResource(texture);


	////SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metaData.mipLevels);







	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU =
		directXCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU =
		directXCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();




	textureSrvHandleCPU.ptr += directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);



	//SRVの生成
	directXCommon_->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);



}




void Object3d::CreateMAterial()
{
	//Resourceにデータを書き込む
	materialResource = object3dCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(Material)); ;


	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color_;
	materialData->enableLighting = false;
	materialData->uvTrasform = MakeIdentity4x4();
}





void Object3d::CreateWVP()
{
	////Resourceにデータを書き込む
	wvpResource = object3dCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(TransformationMatrix)); ;
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));


	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();
}



//void Object3d::CreateTransform()
//{
//}



//void Object3d::CreatLight()
//{
//}


ModelData Object3d::LoadObjFile(const std::string& directoryPath, const std::string& filename)
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
MatrialData Object3d::LoadMatrialTemplateFile(const std::string& directoryPath, const std::string& filename)
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
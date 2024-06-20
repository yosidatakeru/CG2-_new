#include "Sprite.h"
#include "Base.h"

static uint32_t descriptorSizeSRV = 0u;

void Sprite::Initialize(DirectXCommon* directXCommon, SpriteCommon* spriteCommon)
{
	directXCommon_ = directXCommon;
	spriteCommon_ = spriteCommon;

	

	
		//モデル読み込み
		modelData = LoadObjFile("Resources", "plane.obj", modelData);
		modelData2 = LoadObjFile("Resources", "axis.obj", modelData2);
	
	instancingResource =
		spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(TransformationMatrix) * kNumInstance);
	//書き込むアドレスを取得

	instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
	for (uint32_t index = 0; index < kNumInstance; ++index)
	{
		instancingData[index].WVP = MakeIdentity4x4();
		instancingData[index].World = MakeIdentity4x4();
	}

	
	CreateVertex();

	CreatTexture();
	//CreatTexture(modelData2);
	
			descriptorSizeSRV = directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//SRV作成
			D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
			instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
			instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			instancingSrvDesc.Buffer.FirstElement = 0;
			instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			instancingSrvDesc.Buffer.NumElements = kNumInstance;
			instancingSrvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);

			D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU =
				GetCPUDescriptorHandle(directXCommon_->GetSrvDescriptorHeap(), descriptorSizeSRV, 3);
			for (uint32_t index = 0; index < kNumInstance; ++index)
			{
				instancingSrvHandleGPU[index] =
					GetGPUDescriptorHandle(directXCommon_->GetSrvDescriptorHeap(), descriptorSizeSRV, 3);

				directXCommon_->GetDevice()->CreateShaderResourceView(instancingResource.Get(), &instancingSrvDesc, instancingSrvHandleCPU);
			}












			CreateMAterial();

			CreateWVP();



			CreateTransform();

			CreatLight();




}



void Sprite::Update(Transform transform, Transform cameraTransform, Transform transformSprite)
{



	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	wvpData->WVP = worldViewProjectionMatrix;
	wvpData->World = worldMatrix;




	//Sprite用のWorlsViewProjectionMatrixを作る
	Matrix4x4 worudMatrixSprite = MakeAffineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worudMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));

	*transformationMatrixDataSprite = worldViewProjectionMatrixSprite;

	for (uint32_t index = 0; index < kNumInstance; ++index)
	{
		worldMatrix =
		MakeAffineMatrix(transforms[index].scale, transforms[index].rotate, transforms[index].translate);
		worldViewProjectionMatrix =  Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		instancingData[index].WVP = worldViewProjectionMatrix;
		instancingData[index].World = worldMatrix;

	}


	
	

	ImGui::Checkbox("useMonsterBall", &useMonsterBall);


	ImGui::Begin("texture");
	ImGui::DragFloat3("light", &light.x, 0.01f, -1.0f, 1.0f);


	//ImGui::Begin("model");
	
	ImGui::DragFloat3("model", &rotation, 1.0f, -1.0f, 100.0f);

	

	ImGui::End();

	ImGui::Checkbox("uvTrasform", &uvSprite);


	ImGui::Begin("texture");
	ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);


	ImGui::End();


	directionalLighlData->direction = light;

}




void Sprite::Draw(DirectXCommon* directXCommon)
{
	directXCommon_ = directXCommon;


	transform_.translate = {position.x,position.y, 0};
	//回転パラメータ
	transform_.rotate = {0,rotation,0};

	
	for (uint32_t index = 0; index < kNumInstance; ++index)
	{
		transforms[index].scale = { 1.0f, 1.0f, 1.0f };
		transforms[index].rotate = { 0.0f,0.0f, 0.0f };
		transforms[index].translate = { index * 0.1f,  index * 0.1f ,  index * 0.1f };

		
	}
	transforms[1].rotate = { 0.0f, rotation, 0.0f };

	
	materialData->color = color_;

#pragma region コマンドを積む
	

	directXCommon_->GetCommandList()->RSSetViewports(1, directXCommon_->GetViewport());
	
	////RootSignatureを設定。PSOに設定しているけど別途設定が必要
	//ルートシグネチャ
	directXCommon_->GetCommandList()->SetGraphicsRootSignature(spriteCommon_->GetRootSignature());
	
	directXCommon_->GetCommandList()->SetPipelineState(spriteCommon_->GetGraphicsPipelineState());
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	directXCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);//IBVを設定

	////形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	////マテリアルCBufferの場所を設定
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	
	
	
	
	
	//////ライト用
	//directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLighlResource->GetGPUVirtualAddress());

	for (int i = 0; i < instanceCount; i++)
	{
		//////画像
		directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU[i]);


		//////wvp用のCBufferの場所を設定
		//directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
		directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU[i]);
		//directXCommon_->GetCommandList()->DrawInstanced(6, kNumInstance, 0, 0);

		directXCommon_->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), instanceCount, 0, 0);
		//directXCommon_->GetCommandList()->DrawInstanced(UINT(modelData2.vertices.size()), instanceCount, 0, 0);
	}
	
	
	


#pragma endregion
}




void Sprite::Releases()
{
	//indexResourceSprite->Release();
	directionalLighlResource->Release();
	//transformationMatrixResourceSprite->Release();
	//vertexResourceSprite->Release();
	vertexResource->Release();
	materialResourceSprit->Release();
	materialResource->Release();
	for (int i = 0; i < instanceCount; i++)
	{
		wvpResource->Release();
	}
	//textureResource->Release();


	
}




void Sprite::CreateVertex()
{

	//primitive(プリミティブ)
	/*modelData.vertices.push_back({ .position = {1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {0.0f,0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
	modelData.vertices.push_back({ .position = {-1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {1.0f,0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
	modelData.vertices.push_back({ .position = {1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {0.0f,1.0f}, .normal = {0.0f, 0.0f, 1.0f} });
	modelData.vertices.push_back({ .position = {1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {0.0f,1.0f}, .normal = {0.0f, 0.0f, 1.0f} });
	modelData.vertices.push_back({ .position = {-1.0f, 1.0f, 0.0f, 1.0f}, .texcoord = {1.0f,0.0f}, .normal = {0.0f, 0.0f, 1.0f} });
	modelData.vertices.push_back({ .position = {-1.0f, -1.0f, 0.0f, 1.0f}, .texcoord = {1.0f,1.0f}, .normal = {0.0f, 0.0f, 1.0f} });

	modelData.material.textureFilePath = "./Resources/uvChecker.png";
*/

	
	
	//VertexBufferViewを作成
	//頂点バッファビューを作成する
	vertexResource =spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(VertexData) * modelData.vertices.size());

	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size() );
	//１頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);


	////Resourceにデータを書き込む
	//
	////書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());

	










}



void Sprite::CreateMAterial()
{
	
	//Resourceにデータを書き込む
	materialResource = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(Material) ); ;


	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color_;
	materialData->enableLighting = false;
	materialData->uvTrasform = MakeIdentity4x4();



	//Resourceにデータを書き込む
	materialResourceSprit = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(Material)); ;
	//書き込むためのアドレスを取得
	materialResourceSprit->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprit));
	materialDataSprit->color = color_;
	materialDataSprit->enableLighting = false;
	materialDataSprit->uvTrasform = MakeIdentity4x4();

	
	
	
}



void Sprite::CreateWVP()
{
	////////Resourceにデータを書き込む
	wvpResource = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(TransformationMatrix)); ;
	////書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));


	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();



	
}

void Sprite::CreateTransform()
{
	////Sprite用のTransformationMatrix用のリソースを作るMatrix4x4 1とつぶんのサイズを用意する
	transformationMatrixResourceSprite = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(TransformationMatrix));
	
	

	//書き込みのためのアドレスを取得
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));

	//単位行列を書き込む
	*transformationMatrixDataSprite = MakeIdentity4x4();

	
	//Instancing用のTarnsformationMatrixリソースを作る
	


}

void Sprite::CreatLight()
{
	////Resourceにデータを書き込む
	directionalLighlResource = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(DirectionalLigha)); ;

	// directionalLighlData = nullptr;

	//書き込むためのアドレスを取得
	directionalLighlResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLighlData));

	directionalLighlData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLighlData->direction = { 0.0f, -1.0f, 0.0f };
	directionalLighlData->intensity = 1.0f;

}



void Sprite::CreatTexture()
{
	for (uint32_t index = 0; index < textureIndex; ++index)
	{
		////画像読み込み
		std::wstring filePath = ConvertString(modelData.material.textureFilePath);
		DirectX::ScratchImage mipImages = spriteCommon_->LoadTexture(filePath);
		const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
		textureResource[index] = spriteCommon_->CreateTextureResource(directXCommon_->GetDevice(), metaData);


		//画像データを送る
		ID3D12Resource* texture = spriteCommon_->GetIntermediateResource();
		texture = spriteCommon_->UploadTewtureData(textureResource[index], mipImages);
		spriteCommon_->SetIntermediateResource(texture);






		////////SRV作成

		srvDesc[index].Format = metaData.format;
		srvDesc[index].Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc[index].ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
		srvDesc[index].Texture2D.MipLevels = UINT(metaData.mipLevels);

		//SRVを作成するDescriptorHeapの場所を決める
		textureSrvHandleCPU[index] =
			directXCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
		textureSrvHandleGPU[index] =
			directXCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();

		textureSrvHandleCPU[index].ptr += directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		textureSrvHandleGPU[index].ptr += directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//SRVの生成
		directXCommon_->GetDevice()->CreateShaderResourceView(textureResource[index], &srvDesc[index], textureSrvHandleCPU[index]);



		
	}
}




//モデル読み込み関数
ModelData Sprite::LoadObjFile(const std::string& directoryPath, const std::string& filename, ModelData modelData)
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
			s >> texcoord.x >> texcoord.y ;
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
				for (int32_t element = 0; element< 3; ++element)
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
				
				
			
				texcoord.y = 1.0f - texcoord.y;
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

MatrialData Sprite::LoadMatrialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	//1.中で必要となる変数の宣言
	MatrialData matrialData;//構築するマテリアルデータ
	std::string line;//ファイルから読んだ1行目を格納するもの
	
	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);//ファイルを開く
	assert(file.is_open());//とりあえず開かなかったら止める

	//3.実際にファイルを読み込み,NaterialDataを構築する
	while (std::getline(file,line))
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



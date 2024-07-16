#include "Sprite.h"
#include "engine/mata/Base.h"
#include "engine/mata/TextureManager.h"


void Sprite::Initialize(DirectXCommon* directXCommon, SpriteCommon* spriteCommon, std::wstring textureFilePath)
{
	directXCommon_ = directXCommon;
	spriteCommon_ = spriteCommon;
	
	textureIndex = TextureManager::GetInstance()->GetTextureIndexFilePath(textureFilePath);

	CreateVertex();

	//インデクス
	CreateIndex();

	CreateMAterial();

	CreateWVP();

	CreateTransform();
	
	CreatLight();
	
	//画像のサイズ調整
	AdujustTextueSize();

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



	//////Sprite用のWorlsViewProjectionMatrixを作る
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransformSprite.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateXMatrix(uvTransformSprite.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));
	materialDataSprit->uvTrasform = uvTransformMatrix;



	directionalLighlData->direction = light;




	transformSprite_.translate = { position.x,position.y, 0 };
	//回転パラメータ
	transformSprite_.rotate = { rotation.x,rotation.y,rotation.z };



	materialDataSprit->color = color_;

	//サイズ
	transformSprite_.scale = { size.x, size.y, 1.0f };



	//アンカーポイント反映処理
	float left = 0.0f - anchorPoint.x;
	float right = 1.0f - anchorPoint.x;
	float top = 0.0f - anchorPoint.y;
	float bottom = 1.0f - anchorPoint.y;

	//左右反転
	if(isFlipx_== true)
	{
		left = -left;
		right = -right;
	}

	//上下反転
	if(isFlipy_== true)
	{
		top = -top;
		bottom = -bottom;
	}
	const DirectX::TexMetadata& metaData = TextureManager::GetInstance()->GetMetaData(textureIndex);

	float tex_left = textureLeftTop.x / metaData.width;
	float tex_right = (textureLeftTop.x + textureSize.x) / metaData.width;
	float tex_top = textureLeftTop.y / metaData.height;
	float tex_bottom = (textureLeftTop.y + textureSize.y) / metaData.height;


	vertexDataSprite[0].position = { left, bottom, 0.0f, 1.0f }; //左下
	vertexDataSprite[0].texcoord = { tex_left, tex_bottom };

	vertexDataSprite[1].position = { left, top, 0.0f, 1.0f };    //左上
	vertexDataSprite[1].texcoord = { tex_left, tex_top };

	vertexDataSprite[2].position = { right, bottom, 0.0f, 1.0f}; //右下
	vertexDataSprite[2].texcoord = { tex_right, tex_bottom };

	vertexDataSprite[3].position = { right, top, 0.0f, 1.0f };   //右上
	vertexDataSprite[3].texcoord = { tex_right, tex_top };
		
	//
	////ImGui::Checkbox("useMonsterBall", &useMonsterBall);


	ImGui::Begin("texture");
	ImGui::DragFloat3("light", &light.x, 0.01f, -1.0f, 1.0f);


	//ImGui::Begin("model");

	//ImGui::DragFloat3("model", &rotation, 1.0f, -1.0f, 3.0f);

	ImGui::DragFloat3("model", &position.x, 1.0f, -1.0f, 1000.0f);

	//ImGui::DragFloat3("model", &size.x, 1.0f, -1.0f, 1000.0f);


	ImGui::End();

	ImGui::Checkbox("uvTrasform", &uvSprite);


	ImGui::Begin("texture");
	ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);


	ImGui::End();


}




void Sprite::Draw(DirectXCommon* directXCommon)
{
	directXCommon_ = directXCommon;
#pragma region コマンドを積む
	
	directXCommon_->GetCommandList()->RSSetViewports(1, directXCommon_->GetViewport());
    
	//スプライト
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexbufferViewSprite);//VBVの設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	
	directXCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);//IBVを設定
	//マテリアルCBufferの場所を設定
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprit->GetGPUVirtualAddress());
	
    ////TransformationMatrionMatrixCBufferの場所を設定
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite->GetGPUVirtualAddress());
	
	directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetStvHandleGPU(textureIndex));
	
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLighlResource->GetGPUVirtualAddress());
	
	////描画(DrawCall)３兆点で１つのインスタンス。
	directXCommon_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);


#pragma endregion
}




void Sprite::Releases()
{
	directionalLighlResource->Release();
	transformationMatrixResourceSprite->Release();
	vertexResourceSprite->Release();	
	materialResourceSprit->Release();
	wvpResource->Release();
	
}




void Sprite::CreateVertex()
{	
	//Sprite用のの頂点リソースを作る
	vertexResourceSprite = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(VertexData) * 4);

	//リソースの先頭のアドレス
	vertexbufferViewSprite.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点6つぶんのサイズ
	vertexbufferViewSprite.SizeInBytes = sizeof(VertexData) * 4;

	//１頂点当たりのサイズ
	vertexbufferViewSprite.StrideInBytes = sizeof(VertexData);

	//頂点データの設定
	//解放処理していない

	vertexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));

	vertexDataSprite[0].position = { 0.0f, 1.0f, 0.0f, 1.0f };
	vertexDataSprite[0].texcoord = { 0.0f, 1.0f };
	
	//上
	vertexDataSprite[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertexDataSprite[1].texcoord = { 0.0f, 0.0f };
	
	//右下
	vertexDataSprite[2].position = { 1.0f, 1.0f, 0.0f, 1.0f };
	vertexDataSprite[2].texcoord = { 1.0f, 1.0f };
	
	//左下
	vertexDataSprite[3].position = { 1.0f, 0.0f, 0.0f, 1.0f };
	vertexDataSprite[3].texcoord = { 1.0f, 0.0f };
	








}

void Sprite::CreateIndex()
{
	indexResourceSprite =  spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(uint32_t) * 6);

	//リソースの先頭のアドレスから使う
	indexBufferViewSprite.BufferLocation = indexResourceSprite->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 6;
	//１頂点あたりのサイズ
	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;

	//Resourceにデータを書き込む
	uint32_t* indexDataSprite = nullptr;
	//書き込むためのアドレスを取得
	indexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));

	//三角形一枚作成
	indexDataSprite[0] = 0;
	indexDataSprite[1] = 1;
	indexDataSprite[2] = 2;

	//三角形１枚作製
	indexDataSprite[3] = 1;
	indexDataSprite[4] = 3;
	indexDataSprite[5] = 2;
}



void Sprite::CreateMAterial()
{
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
	////Resourceにデータを書き込む
	wvpResource = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(TransformationMatrix)); ;
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();

}

void Sprite::CreateTransform()
{
	//Sprite用のTransformationMatrix用のリソースを作るMatrix4x4 1とつぶんのサイズを用意する
	transformationMatrixResourceSprite = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(TransformationMatrix));
	//書き込みのためのアドレスを取得
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
	//単位行列を書き込む
	*transformationMatrixDataSprite = MakeIdentity4x4();
}

void Sprite::CreatLight()
{
	////Resourceにデータを書き込む
	directionalLighlResource = spriteCommon_->CreateBufferResource(directXCommon_->GetDevice(), sizeof(DirectionalLigha)); ;
	//書き込むためのアドレスを取得
	directionalLighlResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLighlData));
	directionalLighlData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLighlData->direction = { 0.0f, -1.0f, 0.0f };
	directionalLighlData->intensity = 1.0f;
}


void Sprite::AdujustTextueSize()
{
	const DirectX::TexMetadata& metaDeata = TextureManager::GetInstance()->GetMetaData(textureIndex);
	textureSize.x = static_cast<float>(metaDeata.width);
	textureSize.y = static_cast<float>(metaDeata.height);
	size = textureSize;
}



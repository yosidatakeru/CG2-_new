#pragma once
#include"SpriteCommon.h"
#include"engine/base/DirectXCommon.h"
#include"Vector2.h"
#include<numbers>
#include"Matrix3x3.h"
#include"Material.h"
#include"VertexData.h"
#include<fstream>
#include<sstream>
#include"ModelData.h"
#include"MaterialData.h"
class Sprite
{
public:

	


	struct TransformationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct DirectionalLigha
	{
		Vector4 color;//ライトの色
		Vector3 direction;//!<ライトの向き
		float intensity; //!<輝度
	};

	void Initialize(DirectXCommon* directXCommon , SpriteCommon* SpriteCommon_, std::wstring textureFilePath);

	void Draw(DirectXCommon* directXCommon);
	void Update(Transform transform, Transform cameraTransform, Transform transformSprite);
	
	void Releases();
private:

	//頂点データの作成関数
	void CreateVertex();

	void CreateIndex();

	//マテリアルの作成関数
	void CreateMAterial();

	//行列情報の作成
	//マテリアルの作成関数
	void CreateWVP();


	//2D用のTransform
	void CreateTransform();

	void CreatLight();

	
	//本来テクスチャサイズを合わせる
	void AdujustTextueSize();
public:

	ID3D12Resource* GetwvpResource() const { return  wvpResource; }

	//色
	Vector4 GetColor()           { return color_; }
	void SetColor(Vector4& color) { color_ = color; }

	//回転
	float GetRotation() { return rotation; }
	void SetRotaion(float rot) { this->rotation = rot; }

	//移動
	Vector2 GetPosition() { return position; }
	void SetPosintion(Vector2 pos) { this->position = pos; }

	//移動
	Vector2 GetSize() { return size; }
	void SetSize(Vector2 size) { this->size = size; }


	Vector3 GetCameraPosition() { return camerPosition; }
	void SetCameraPosintion(Vector3 cameraPos) { camerPosition = cameraPos; }

	Vector2 GetAnchorPoint()  { return anchorPoint; }
	void SetAnchorPoint(Vector2 anchorPoint) { this->anchorPoint = anchorPoint; }

	//左右反転
	bool GetIsFlipX() { return  isFlipx_; }
	//上下反転
	bool GetIsFlipY() { return  isFlipy_; }

	
	//左右反転
	void SetIsFlipX(bool isFlag) { isFlag = isFlag; }

	//上下反転
	void SetIsFlipY(bool isFlag) { isFlag = isFlag; }


	Transform GetTransform() { return  transform_; }

	Transform GetCameraTransform() { return  cameraTransform_; }
	   
	Transform GetTransformSprite() { return  transformSprite_; }


	//切り抜き関連
	Vector2 GetTextureLeftTop() { return textureLeftTop; }
	Vector2 GetTextureSize() { return textureSize; }

	//切り抜き関連
	void SetTextureLeftTop(Vector2 value) { textureLeftTop = value; }
	void SetTextureSize(Vector2 size) { textureSize = size; }
private:

	float rotation = 0;

	////色パラメーター
	Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};

	
	Transform transform_ { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	
	Transform cameraTransform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };

	 Vector2 position = { 0, 0 };

	 Vector3 camerPosition = { 0, 0, 0 };

	 Transform transformSprite_{ {1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	 Vector2 size = { 0.0f, 0.0f };

	 Vector2 anchorPoint = { 0.0f, 0.0f };

	 Transform uvTransformSprite
	 {
		 {1.0f, 1.0f, 1.0f},
		 {0.0f, 0.0f, 0.0f},
		 {0.0f, 0.0f, 0.0f},
	 };

private:

	DirectXCommon* directXCommon_ = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	
	uint32_t textureIndex = 0;

	////頂点バッファリソーソを作る
	ID3D12Resource* vertexResourceSprite = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexbufferViewSprite{};
	

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSprite = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};
	
	ID3D12Resource* materialResourceSprit = nullptr;
	Material* materialDataSprit = nullptr;



	ID3D12Resource* wvpResource = nullptr;
	TransformationMatrix* wvpData = nullptr;


	//////データの書き込み
	Matrix4x4* transformationMatrixDataSprite = nullptr;
	ID3D12Resource* transformationMatrixResourceSprite = nullptr;



	ID3D12Resource* directionalLighlResource = nullptr;
	DirectionalLigha* directionalLighlData = nullptr;
	Vector3 light = { 0.0f, -1.0f,0.0f };


	//切り抜きたい画像内の座標
	Vector2 textureLeftTop = { 0,0 };
	//切り抜きたい画像内のサイズ
	Vector2 textureSize = { 0, 0 };

	// //フリップ
	 bool isFlipx_ = false;
	 bool isFlipy_ = false;

	 VertexData* vertexDataSprite = nullptr;


	 bool uvSprite = true;

	

	
	
};


#pragma once
#include"base/SpriteCommon.h"
#include"base/DirectXCommon.h"
#include"Vector2.h"
class Sprite
{
public:

	struct  VertexData
	{
		Vector4 position;
		Vector2 texcoord;
	};


	void Initialize(DirectXCommon* directXCommon , SpriteCommon* SpriteCommon_ );

	void Draw(DirectXCommon* directXCommon);
	void Update(Transform transform, Transform cameraTransform);
	
	void Releases();
private:

	//頂点データの作成関数
	void CreateVertex();

	//マテリアルの作成関数
	void CreateMAterial();

	//行列情報の作成
	//マテリアルの作成関数
	void CreateWVP();

public:

	ID3D12Resource* GetwvpResource() const { return  wvpResource; }

	//色
	Vector4 GetColor()           { return color_; }
	void SetColor(Vector4 color) { color_ = color; }

	//回転
	float GetRotation() { return rotation; }
	void SetRotaion(float rot) { rotation = rot; }

	//移動
	Vector2 GetPosition() { return position; }
	void SetPosintion(Vector2 pos) { position = pos; }

	Vector3 GetCameraPosition() { return camerPosition; }
	void SetCameraPosintion(Vector3 cameraPos) { camerPosition = cameraPos; }


	Transform GetTransform() { return  transform_; }

	Transform GetCameraTransform() { return  cameraTransform_; }
	
private:
	VertexData* vertexData = nullptr;
	
	ID3D12Resource* textureResource = nullptr;
	
	Matrix4x4* wvpData = nullptr;

	Vector4* materialData = nullptr;

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXCommon* directXCommon_ = nullptr;
	ID3D12Resource* vertexResource = nullptr;

	ID3D12Resource* materialResource = nullptr;

	ID3D12Resource* wvpResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	float rotation = 0;

	//色パラメーター
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	Transform transform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	
	Transform cameraTransform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };

	 Vector2 position = { 0, 0 };

	 Vector3 camerPosition = { 0, 0, 0 };

	 //画像の保蔵先のアドレス
	 D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

};


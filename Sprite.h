#pragma once
#include"base/SpriteCommon.h"
#include"base/DirectXCommon.h"
#include"Vector2.h"
#include<numbers>
#include"base/Matrix3x3.h"
class Sprite
{
public:

	struct  VertexData
	{
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct Material
	{
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTrasform;
	    
		float shininess;
	};

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

	void Initialize(DirectXCommon* directXCommon , SpriteCommon* SpriteCommon_ );

	void Draw(DirectXCommon* directXCommon);
	void Update(Transform transform, Transform cameraTransform, Transform transformSprite);
	
	void Releases();
private:

	//頂点データの作成関数
	void CreateVertex();

	//マテリアルの作成関数
	void CreateMAterial();

	//行列情報の作成
	//マテリアルの作成関数
	void CreateWVP();


	//2D用のTransform
	void CreateTransform();

	void CreatLight();


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
	   
	Transform GetTransformSprite() { return  transformSprite; }
private:
	VertexData* vertexData = nullptr;
	
	ID3D12Resource* textureResource = nullptr;
	ID3D12Resource* textureResource2 = nullptr;
	

	TransformationMatrix* wvpData = nullptr;

	Material* materialData = nullptr;
	Material* materialDataSprit = nullptr;

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXCommon* directXCommon_ = nullptr;
	ID3D12Resource* vertexResource = nullptr;
	ID3D12Resource* indexResourceSprite = nullptr;


	ID3D12Resource* materialResource = nullptr;
	ID3D12Resource* materialResourceSprit = nullptr;

	ID3D12Resource* wvpResource = nullptr;
	
	ID3D12Resource* vertexResourceSprite = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	////データの書き込み
	Matrix4x4* transformationMatrixDataSprite = nullptr;

	ID3D12Resource* transformationMatrixResourceSprite = nullptr;

	VertexData* vertexDataSprite = nullptr;

	//頂点バッファリソーソを作る
	D3D12_VERTEX_BUFFER_VIEW vertexbufferViewSprite{};

	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};


	float rotation = 0;

	//色パラメーター
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	Transform transform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	
	Transform cameraTransform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };

	 Vector2 position = { 0, 0 };

	 Vector3 camerPosition = { 0, 0, 0 };

	 Transform transformSprite{ {1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	 Transform uvTransformSprite
	 {
		 {1.0f, 1.0f, 1.0f},
		 {0.0f, 0.0f, 0.0f},
		 {0.0f, 0.0f, 0.0f},
	 };

	 //画像の保蔵先のアドレス
	 D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	 D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2;

	 //円形描画用
	 const uint32_t kSubdivision = 12;
	 const uint32_t kNumSphereVerices = kSubdivision * kSubdivision * 6;
	 float pi = std::numbers::pi_v<float>;

	 bool useMonsterBall = true;

	 bool uvSprite = true;
	 D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap * descriptoHeap, uint32_t descriptorSize, uint32_t index)
	 {
		 D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptoHeap->GetCPUDescriptorHandleForHeapStart();
		 handleCPU.ptr += (descriptorSize * index);
		 return handleCPU;
	 }

	 D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
	 {
		 D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		 handleGPU.ptr += (descriptorSize * index);
		 return handleGPU;
	 }
	 ID3D12Resource* directionalLighlResource = nullptr;
	 DirectionalLigha* directionalLighlData = nullptr;
	 Vector3 light = { 0.0f, -1.0f,0.0f };
};


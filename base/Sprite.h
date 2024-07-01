#pragma once
#include"SpriteCommon.h"
#include"DirectXCommon.h"
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

	void CreateTexture(std::wstring textureFilePath);

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
	VertexData* vertexData = nullptr;
	

	ID3D12Resource* textureResource = nullptr;
	

	TransformationMatrix* wvpData = nullptr;

	Material* materialData = nullptr;
	Material* materialDataSprit = nullptr;

	SpriteCommon* spriteCommon_ = nullptr;
	DirectXCommon* directXCommon_ = nullptr;
	ID3D12Resource* vertexResource = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSprite = nullptr;


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
	//
	
	ModelData modelData; // 構築するMaterualData

	


	float rotation = 0;

	//色パラメーター
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
	 //切り抜きたい画像内の座標
	 Vector2 textureLeftTop = { 0,0 };
	 //切り抜きたい画像内のサイズ
	  Vector2 textureSize = { 0, 0 };

	 uint32_t textureIndex = 0;

	 //フリップ
	 bool isFlipx_ = false;
	 bool isFlipy_ = false;

	 Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	 D3D12_INDEX_BUFFER_VIEW indexBufferView{};
};


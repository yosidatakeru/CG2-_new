#pragma once
#include"engine/mata/Vector3.h"
#include"engine/mata/Vector2.h"
#include"engine/mata/Transform.h"
#include"engine/mata/Matrix4x4.h"
#include"engine/base/Input.h"
class Camera 
{
public:

	void Initialize();

	void CameraUpdate();

	Matrix4x4  ViewMatrix();
	Matrix4x4  ProjectionMatrix();

protected:
	

	

public:
	//回転
	Vector3 GetCameraRotation() { return rotation; }
	void SetCameraRotaion(Vector3 rot){rotation = rot;}

	//移動
	Vector3 GetCameraPosition() { return position; }
	void SetCameraPosintion(Vector3 pos) { position = pos; }

	
	Vector2 GetCameraSize() { return size; }
	void SetCameraSize(Vector2 size) { size = size; }

	// ビュー行列を取得
	Matrix4x4 GetViewMatrix() const { return viewMatrix; }
	
	// プロジェクション行列を取得
	Matrix4x4 GetProjectionMatrix() const { return projectionMatrix; }

	Transform GetcameraTransform() const { return cameraTransform_; }
	

private:

	Transform cameraTransform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };



	Vector3 position = { 0.0f, 0.0f,-40.0f };

	
	Vector3 rotation = { 0.0f, 0.0f, 0.0f };
	

	Vector2 size = { 1280.0f, 720.0f };


	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;

	Matrix4x4 cameraMatrix;

	float fov = 0.45f; // 視野角（フィールドオブビュー）
	float aspectRatio = size.x / size.y; // アスペクト比
	float nearPlane = 0.1f; // 近接平面
	float farPlane = 100.0f; // 遠方平面
	
};


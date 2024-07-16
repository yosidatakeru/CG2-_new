#include "Camera.h"
#include "engine/mata/Matrix4x4CalCulation.h"
#include "engine/base/WinApp.h"


//コンストラクタ
Camera::Camera() {
	//コンストラクタの所で値を入れる
	//わざわざInitialize関数を作るのは面倒だから
	//デフォルト
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-9.8f} };
}

//インスタンス
Camera* Camera::GetInstance() {
	//関数内static変数として宣言する
	static Camera instance;

	return &instance;

}

Matrix4x4 Camera::GetAffineMatrix() {
	cameraMatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	return cameraMatrix_;
}

Matrix4x4 Camera::GetViewMatrix() {
	//カメラ行列
	cameraMatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	return viewMatrix_;
}

Matrix4x4 Camera::GetProjectionMatrix_() {
	//遠視投影行列
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);

	return projectionMatrix_;
}




//デストラクタ
Camera::~Camera() {

}


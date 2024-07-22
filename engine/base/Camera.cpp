#include "Camera.h"
#include <engine/mata/MyFunction.h>
void Camera::Initialize()
{
  
   


     
}

void Camera::CameraUpdate()
{
    
    cameraTransform_.translate = { position.x, position.y, position.z };


    cameraTransform_.rotate = { rotation.x, rotation.y, rotation.z };
   
   
 
   
}

Matrix4x4  Camera::ViewMatrix()
{
    
    cameraMatrix = MakeAffineMatrix(cameraTransform_.scale,cameraTransform_.rotate,cameraTransform_.translate);
    viewMatrix = Inverse(cameraMatrix);
    
    return viewMatrix;
}

Matrix4x4  Camera::ProjectionMatrix()
{
    
    projectionMatrix = MakePerspectiveFovMatrix(fov, aspectRatio, nearPlane, farPlane);
    return  projectionMatrix;
}

#include "Camera.h"

XMFLOAT3 Camera::cameraPos;
XMFLOAT3 Camera::up;
XMFLOAT3 Camera::target;
XMFLOAT3 Camera::pPos;
float Camera::eyeangleX = 0;
float Camera::eyeangleY = 0;
float Camera::eyeangleZ = 0;

Camera::Camera()
{
		cameraPos = XMFLOAT3(0, 45, -75);
		target =  XMFLOAT3(0, 0, 0);
		up = XMFLOAT3(0, 1, 0);
	
}
void Camera::Initialize()
{
	cameraPos = XMFLOAT3(0, 30, -25);


 }
XMFLOAT3 Camera::CameraPos()
{
	XMFLOAT3 _cameraPos;

	_cameraPos = XMFLOAT3(cameraPos.x*cos(eyeangleY) - cameraPos.z*sin(eyeangleY), cameraPos.y, cameraPos.x*sin(eyeangleY) + cameraPos.z*cos(eyeangleY));
	_cameraPos.x += pPos.x;
	_cameraPos.y += pPos.y;
	_cameraPos.z += pPos.z;

	
	return _cameraPos ;
	
}

XMFLOAT3 Camera::Target()
{
	target.y + 15;
	return target;
}

XMFLOAT3 Camera::Up()
{
	return up;
}

void Camera::SetEye(XMFLOAT3 _targetPos)
{
	pPos = _targetPos;
}

void Camera::SetTarget(XMFLOAT3 _target)
{
	target = _target;
}

void Camera::SetUp(XMFLOAT3 _up)
{
	up = _up;
}

void Camera::SetCameraPos(XMFLOAT3 _cameraPos)
{
	cameraPos = _cameraPos;
}

XMFLOAT3 Camera::NormalizeXZ(XMFLOAT3 vec)
{
	float len = sqrtf(vec.x*vec.x + vec.z*vec.z);
	return XMFLOAT3(vec.x / len, 0, vec.z / len);
}





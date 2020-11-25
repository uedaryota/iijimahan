#include "Camera.h"


CameraState Camera::camera1;
CameraState Camera::camera2;
CurrentCamera Camera::currentcamera;
Camera::Camera()
{
	camera1.cameraPos = XMFLOAT3(0, 65, -175);
	camera1.target = XMFLOAT3(0, 0, 0);
	camera1.up = XMFLOAT3(0, 1, 0);
	
	camera1.matView = XMMatrixLookAtLH(
		XMLoadFloat3(&Camera::MainCameraPos()), XMLoadFloat3(&Camera::Target()), XMLoadFloat3(&Camera::Up())
	);


	camera1.matProjection = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast<float>(Camera::window_width) / static_cast<float>(Camera::window_height),
		1.0f,
		10000.0f
	);

	camera2.cameraPos = XMFLOAT3(0, 165, -375);
	camera2.target = XMFLOAT3(0, 0, 0);
	camera2.up = XMFLOAT3(0, 1, 0);
	camera2.matView = XMMatrixLookAtLH(
		XMLoadFloat3(&Camera::SubCameraPos()), XMLoadFloat3(&Camera::ReturnSubCameraState()->target), XMLoadFloat3(&Camera::Up())
	);


	camera2.matProjection = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast<float>(Camera::window_width) / static_cast<float>(Camera::window_height),
		1.0f,
		10000.0f
	);

	currentcamera = CurrentCamera::Main;
}
void Camera::Initialize()
{
	camera1.cameraPos = XMFLOAT3(0, 30, -25);
 }
void Camera::Update()
{
	camera1.matView = XMMatrixLookAtLH(
		XMLoadFloat3(&Camera::MainCameraPos()), XMLoadFloat3(&Camera::Target()), XMLoadFloat3(&Camera::Up())
	);

	camera2.matView = XMMatrixLookAtLH(
		XMLoadFloat3(&Camera::SubCameraPos()), XMLoadFloat3(&Camera::ReturnSubCameraState()->target), XMLoadFloat3(&Camera::Up())
	);


}
XMFLOAT3 Camera::MainCameraPos()
{
	if (currentcamera == CurrentCamera::Main)
	{
		XMFLOAT3 _cameraPos;

		_cameraPos = XMFLOAT3(camera1.cameraPos.x*cos(camera1.eyeangleY) - camera1.cameraPos.z*sin(camera1.eyeangleY),
			camera1.cameraPos.y, camera1.cameraPos.x*sin(camera1.eyeangleY) + camera1.cameraPos.z*cos(camera1.eyeangleY));
		_cameraPos.x += camera1.pPos.x;
		_cameraPos.y += camera1.pPos.y;
		_cameraPos.z += camera1.pPos.z;


		return _cameraPos;
	}
	if (currentcamera == CurrentCamera::Sub)
	{
		XMFLOAT3 _cameraPos;

		_cameraPos = XMFLOAT3(camera2.cameraPos.x*cos(camera2.eyeangleY) - camera2.cameraPos.z*sin(camera2.eyeangleY),
			camera2.cameraPos.y, camera2.cameraPos.x*sin(camera2.eyeangleY) + camera2.cameraPos.z*cos(camera2.eyeangleY));
		_cameraPos.x += camera2.pPos.x;
		_cameraPos.y += camera2.pPos.y;
		_cameraPos.z += camera2.pPos.z;


		return _cameraPos;
	}
}
//ÉÅÉCÉìÇ∂Ç·Ç»Ç¢ÇŸÇ§Çï‘Ç∑
XMFLOAT3 Camera::SubCameraPos()
{
	
	if (currentcamera == CurrentCamera::Sub)
	{
		XMFLOAT3 _cameraPos;

		_cameraPos = XMFLOAT3(camera1.cameraPos.x*cos(camera1.eyeangleY) - camera1.cameraPos.z*sin(camera1.eyeangleY),
			camera1.cameraPos.y, camera1.cameraPos.x*sin(camera1.eyeangleY) + camera1.cameraPos.z*cos(camera1.eyeangleY));
		_cameraPos.x += camera1.pPos.x;
		_cameraPos.y += camera1.pPos.y;
		_cameraPos.z += camera1.pPos.z;


		return _cameraPos;
	}
	else if (currentcamera == CurrentCamera::Main)
	{
		XMFLOAT3 _cameraPos;

		_cameraPos = XMFLOAT3(camera2.cameraPos.x*cos(camera2.eyeangleY) - camera2.cameraPos.z*sin(camera2.eyeangleY),
			camera2.cameraPos.y, camera2.cameraPos.x*sin(camera2.eyeangleY) + camera2.cameraPos.z*cos(camera2.eyeangleY));
		_cameraPos.x += camera2.pPos.x;
		_cameraPos.y += camera2.pPos.y;
		_cameraPos.z += camera2.pPos.z;


		return _cameraPos;
	}
}

XMFLOAT3 Camera::Target()
{
	camera1.target.y + 15;
	return camera1.target;
}

XMFLOAT3 Camera::Up()
{
	return camera1.up;
}

void Camera::SetEye(XMFLOAT3 _targetPos)
{
	camera1.pPos = _targetPos;
}

void Camera::SetTarget(XMFLOAT3 _target)
{
	camera1.target = _target;
}

void Camera::SetUp(XMFLOAT3 _up)
{
	camera1.up = _up;
}

void Camera::SetCameraPos(XMFLOAT3 _cameraPos)
{
	camera1.cameraPos = _cameraPos;
}

void Camera::ChangeMainCamera()
{
	if (currentcamera == CurrentCamera::Main)
	{
		currentcamera = CurrentCamera::Sub;
	}
	if (currentcamera == CurrentCamera::Sub)
	{
		currentcamera = CurrentCamera::Main;
	}
}

XMFLOAT3 Camera::NormalizeXZ(XMFLOAT3 vec)
{
	float len = sqrtf(vec.x*vec.x + vec.z*vec.z);
	return XMFLOAT3(vec.x / len, 0, vec.z / len);
}

CameraState* Camera::ReturnCameraState()
{
	if (currentcamera == CurrentCamera::Main)
	{
		return &camera1;
	}
	else if (currentcamera == CurrentCamera::Sub)
	{
		return &camera2;
	}
}

CameraState *Camera::ReturnSubCameraState()
{
	if (currentcamera == CurrentCamera::Main)
	{
		return &camera2;
	}
	else if (currentcamera == CurrentCamera::Sub)
	{
		return &camera1;
	}
}





#pragma once
#pragma once
#include<Windows.h>
#ifdef _DEBUG
#include<iostream>
#endif
#include<d3d12.h>
#include<dxgi1_6.h>
#include<vector>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#include<DirectXTex.h>
#include<d3dx12.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace std;
using namespace DirectX;

struct CameraState
{
	XMFLOAT3 cameraPos;
	XMFLOAT3 target;
	XMFLOAT3 up;
	XMFLOAT3 pPos;
	float eyeangleX;
	float eyeangleY;
	float eyeangleZ;
	XMMATRIX matView;
	XMMATRIX matProjection;
};
static enum CurrentCamera
{
	Main,Sub,
};
static class Camera
{
	
	
public:


	static const int window_width = 1280;
	static const int window_height = 720;

	Camera();
	void Initialize();
	static void Update();
	static XMFLOAT3 MainCameraPos();
	static XMFLOAT3 SubCameraPos();

	static  XMFLOAT3 Target();
	static  XMFLOAT3 Up();
	static  void SetEye(XMFLOAT3 eye);
	static void SetTarget(XMFLOAT3 target);
	static  void SetUp(XMFLOAT3 up);
	static  void SetCameraPos(XMFLOAT3 cameraPos);
	static void ChangeMainCamera();
	static XMFLOAT3 NormalizeXZ(XMFLOAT3 vec);
	static CameraState* ReturnCameraState();
	static CameraState* ReturnSubCameraState();


private:

	static CameraState camera1;
	static CameraState camera2;

	static CurrentCamera currentcamera;
	
};


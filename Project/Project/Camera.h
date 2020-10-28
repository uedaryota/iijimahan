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
static class Camera
{


	
public:
	static const int window_width = 1280;
	static const int window_height = 720;

	Camera();
	void Initialize();
	static XMFLOAT3 CameraPos();
	static  XMFLOAT3 Target();
	static  XMFLOAT3 Up();
	static  void SetEye(XMFLOAT3 eye);
	static void SetTarget(XMFLOAT3 target);
	static  void SetUp(XMFLOAT3 up);
	static  void SetCameraPos(XMFLOAT3 cameraPos);

	XMFLOAT3 NormalizeXZ(XMFLOAT3 vec);
	static float eyeangleX;
	static float eyeangleY;
	static float eyeangleZ;

public:
	static XMFLOAT3 cameraPos;
	static XMFLOAT3 target;
	static XMFLOAT3 up;
	static XMFLOAT3 pPos;

	
};


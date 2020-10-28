#pragma once
#include<Windows.h>
#ifdef _DEBUG
#include<iostream>
#endif
#include<d3d12.h>
#include <wrl.h>
#include<dxgi1_6.h>
#include<vector>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#include"Texture.h"
#include"PMDClass.h"
#include"Floor.h"
#include"Camera.h"
#include"Poly.h"
#include"Sprite.h"
#include"ObjFile.h"
#include"Tower.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace std;
using namespace DirectX;

static class DirectXDevice
{
private:
public:
	static  ID3D12GraphicsCommandList* cmdList ;
	static ID3D12Device* dev;
};
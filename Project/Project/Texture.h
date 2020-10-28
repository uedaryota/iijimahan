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
#include"Camera.h"
using namespace std;
using namespace DirectX;
class Texture
{
	public:
		Texture();
		void Initialize(ID3D12Device* dev, Camera* camera);
		void Update();
		ID3D12PipelineState* texpipelinestate = nullptr;
		ID3D12RootSignature* texrootsignature = nullptr;
		ID3D12DescriptorHeap* texDescHeap;
		ID3D12DescriptorHeap* dsvHeap = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
	
private:
	void SetPipe_Rootsig(ID3D12Device* dev);
	void SetDepth(ID3D12Device* dev);
	
	void SetVert(ID3D12Device* dev);

	const int window_width = 1280;
	const int window_height = 720;
	float angle = 0;
	struct MatrocesDate
	{
		XMMATRIX world;
		XMMATRIX viewproj;
	};
	Camera* c;
	XMMATRIX matWorld = XMMatrixIdentity();
	XMMATRIX matView;
	ID3D12Resource* constBuff = nullptr;
	XMMATRIX matProjection;
	MatrocesDate* constMap;
	XMFLOAT3 eye=XMFLOAT3(0, 25, -25);
	XMFLOAT3 target = XMFLOAT3(0, 0, 0);
	XMFLOAT3 up = XMFLOAT3(0, 1, 0);

	struct  Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

   
	Vertex vertices[4] =
	{
		{{-1.0f,-1.0f,0},{0.0f,1.0f}},
	{{-1.0f,1.0f,0},{0.0f,0.0f} },
	{{1.0f,-1.0f,0} , { 1.0f,1.0f }},
	{{1.0f,1.0f,0} , { 1.0f,0.0f }},

	};

	unsigned short indices[6] = {
		0,1,2,
		2,1,3
	};

};


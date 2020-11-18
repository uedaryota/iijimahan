#pragma once
#include<Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include<DirectXTex.h>
#include <vector>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include<stdio.h>
#include<d3dx12.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInput のバージョン指定
#include <dinput.h> 
#include"Camera.h"
#include"Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
class Poly
{
	struct  Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};
	struct MatrocesDate
	{
		XMMATRIX world;
		XMMATRIX viewproj;
	};
public:
	Poly();
	~Poly();
	void Initialize(ID3D12Device* dev);
	void CreatePipeline(ID3D12Device* dev);
	void CreateMainHeap(ID3D12Device* dev);
	void SetDepth(ID3D12Device* dev);
	void SetVert(ID3D12Device* dev);
	void Draw(ID3D12GraphicsCommandList* cmdList, ID3D12Device* dev);
	void Update();
	void SetPos(XMFLOAT3 pos);
	void SetScale(XMFLOAT3 Scale);
	ID3D12PipelineState* pipelinestate = nullptr;
	ID3D12RootSignature* rootsignature = nullptr;
	ID3D12DescriptorHeap* mainDescHeap;
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};

	D3D12_INDEX_BUFFER_VIEW ibView{};

	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 rotation = { XM_PIDIV2,0,0 };
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };

	
	static const int n = 4;
	Vertex vertices[(n + 1) * 2];

	unsigned short indices[n * 4 * 3];



private:
	XMMATRIX matWorld = XMMatrixIdentity();
	XMMATRIX matView;
	ID3D12Resource* constBuff = nullptr;
	XMMATRIX matProjection;
	MatrocesDate* constMap;

	XMMATRIX matScale;
	XMMATRIX matRot;
	XMMATRIX matTrans;
	Input* input;
};


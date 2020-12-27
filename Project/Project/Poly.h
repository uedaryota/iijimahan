#pragma once
#include <vector>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include"Camera.h"
#include"Input.h"
using namespace DirectX;
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
	void Initialize();
	void CreatePipeline();
	void CreateMainHeap();
	void CreateSubHeap();
	void SetDepth();
	void SetVert();
	void Draw();
	void Update();
	void SetPos(XMFLOAT3 pos);
	void SetScale(XMFLOAT3 Scale);
	void SetRotate(XMFLOAT3 rotate);
	ID3D12PipelineState* pipelinestate = nullptr;
	ID3D12RootSignature* rootsignature = nullptr;
	ID3D12DescriptorHeap* mainDescHeap;
	ID3D12DescriptorHeap* subDescHeap;

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
	ID3D12Resource* constBuff = nullptr;
	MatrocesDate* constMap;

	ID3D12Resource* constBuff2 = nullptr;
	MatrocesDate* constMap2;

	XMMATRIX matScale;
	XMMATRIX matRot;
	XMMATRIX matTrans;
	Input* input;
};


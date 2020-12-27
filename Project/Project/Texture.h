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
		void Initialize();
		void Update();
		void Draw();
		void ResetTex(const wchar_t* Texname);
		ID3D12PipelineState* texpipelinestate = nullptr;
		ID3D12RootSignature* texrootsignature = nullptr;
		ID3D12DescriptorHeap* texDescHeap;
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		D3D12_INDEX_BUFFER_VIEW ibView{};

		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT3 rotation = { 0,0,0 };
		XMFLOAT3 scale = { 1,1,1 };
private:
	void SetPipe_Rootsig(ID3D12Device* dev);

	void SetVert(ID3D12Device* dev);

	XMMATRIX matScale;
	XMMATRIX matRot;
	XMMATRIX matTrans;
	struct MatrocesDate
	{
		XMMATRIX world;
		XMMATRIX viewproj;
	};

	XMMATRIX matWorld = XMMatrixIdentity();
	ID3D12Resource* constBuff = nullptr;
	MatrocesDate* constMap;

	struct  Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

   
	Vertex vertices[4] =
	{
		{{-0.5f,-0.5f,0},{0.0f,1.0f}},
	{{-0.5f,0.5f,0},{0.0f,0.0f} },
	{{0.5f,-0.5f,0} , { 1.0f,1.0f }},
	{{0.5f,0.5f,0} , { 1.0f,0.0f }},

	};

	unsigned short indices[6] = {
		0,1,2,
		2,1,3
	};

};


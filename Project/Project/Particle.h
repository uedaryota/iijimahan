#pragma once
#include <d3d12.h>
#include<DirectXTex.h>
#include <vector>
#include <DirectXMath.h>
using namespace DirectX;
class Particle
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
		XMFLOAT4 color;
	};

public:
	Particle();
	~Particle();
	void Initialize();
	void CreatePipeline();
	void CreateMainHeap();
	void SetTex(const wchar_t* texname);
	void SetVert();
	void Draw();
	void Update();
	void SetPos(XMFLOAT3 pos);
	void SetScale(XMFLOAT3 Scale);
	XMMATRIX SetBillboard();
	static	ID3D12PipelineState* pipelinestate;
	static	ID3D12RootSignature* rootsignature ;
	//static	ID3D12DescriptorHeap* mainDescHeap;
	ID3D12DescriptorHeap* mainDescHeap;
	static D3D12_VERTEX_BUFFER_VIEW vbView ;

	static D3D12_INDEX_BUFFER_VIEW ibView;

	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 velocity = { 0,0,0 };
	float speed = 1;
	float addspeed = 0;
	float life = 15;
	float maxColor = 1;
	float minColor = 0;

	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 scale = { 100.0f,100.0f,100.0f };
	XMFLOAT4 color;
private:
	XMMATRIX matWorld = XMMatrixIdentity();
	static XMMATRIX matView;

	ID3D12Resource* constBuff = nullptr;
	MatrocesDate* constMap = nullptr;

	//static const int size = 0.5f;
	static Vertex vertices[4];

	static unsigned short indices[6];
	XMMATRIX matScale;
	XMMATRIX matRot;
	XMMATRIX matTrans;

	static	ID3D12Resource* vertBuff;
	static	ID3D12Resource* indexBuff;
	static ID3D12Resource* texBuff;

	static Vertex* vertMap;
	static unsigned short* indexMap;
};


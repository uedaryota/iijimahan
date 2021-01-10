#pragma once
#include<DirectXMath.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#include<DirectXTex.h>
#include<d3dx12.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

//#include"DirectXDevice.h"
using namespace DirectX;
class CircleCollision
{
	struct  Vertex
	{
		XMFLOAT3 pos;
	};
	struct ConstDate
	{
		XMMATRIX world;
		XMMATRIX viewproj;
		XMFLOAT4 color;
	};
public:
	CircleCollision();
	~CircleCollision();
	void Update();
	bool CollisionUpdate(CircleCollision other);//ìñÇΩÇËîªíË
	void SetMainHeap();
	void SetPipeLine();
	void SetMatrix();
	void SetVert();
	void Draw();//ï`âÊ
	void Initialize();
	void SetPos(XMFLOAT3 pos);//ç¿ïWïœçX
	void AddPos(XMFLOAT3 pos);
	void SetLength(float len);//îºåaïœçX
	XMFLOAT4 color = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5);
	float scale = 10;
	XMFLOAT3 position = { 0,0,0 };//ç¿ïW
	XMFLOAT3 rotation = { 0,0,0 };
private:
	float length = 0.5f;//îºåa
	//ï`âÊê›íË

	ID3D12PipelineState* pipelinestate = nullptr;
	ID3D12RootSignature* rootsignature = nullptr;
	ID3D12DescriptorHeap* mainDescHeap;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//
	static const int vertnum = 100;
	Vertex vertices[vertnum + 1];

	unsigned short indices[vertnum * 3];


	XMMATRIX matScale;
	XMMATRIX matRot;
	XMMATRIX matTrans;
	XMMATRIX matWorld = XMMatrixIdentity();
	XMMATRIX matView;
	XMMATRIX matProjection;
	ID3D12Resource* vertBuff;
	ID3D12Resource* indexBuff = nullptr;
	ID3D12Resource* constBuff = nullptr;

	XMMATRIX matBillboard;
	ConstDate* constMap;
	Vertex* vertMap = nullptr;
	unsigned short* indexMap = nullptr;

};
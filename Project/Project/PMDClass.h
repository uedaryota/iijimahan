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
#include"Floor.h"

#include"Input.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class PMDClass
{
	struct PMDHeader
	{
		float version;
		char model_name[20];
		char comment[256];
	};
	struct PMDVertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
		unsigned short boneNo[2];
		unsigned char boneWeight;
		unsigned char edgeFlg;
	};
	#pragma pack(1)
	struct PMDMaterial
	{
		XMFLOAT3 diffuse;
		float alpha;
		float specularity;
		XMFLOAT3 specular;
		XMFLOAT3 ambient;
		unsigned char toonIdx;
		unsigned char edgeFlag;
		unsigned int indicesNum;
		char texFilePath[20];
	};
#pragma pack()
	//
	struct MaterialForhlsl
	{
		XMFLOAT3 diffuse;
		float alpha;
		XMFLOAT3 specular;
		float speculaty;
		XMFLOAT3 ambient;
	};
	struct AdditionalMaterial
	{
		std::string texPath;
		int toonIdx;
		bool edgeFlag;
	};

	struct Material
	{
		unsigned int indicesNum;
		MaterialForhlsl material;
		AdditionalMaterial additional;

	};
	//
public:
	PMDClass();
	~PMDClass();
	void Initialize(ID3D12Device* dev, Camera* camera);
	void CreatePipeline(ID3D12Device* dev);
	void CreateMainHeap(ID3D12Device* dev);
	void SetDepth(ID3D12Device* dev);
	void Draw(ID3D12GraphicsCommandList* cmdList, ID3D12Device* dev);
	void Update();
	void CollisionFloor(Floor floor);
public:
	ID3D12PipelineState* PMDpipelinestate = nullptr;
	ID3D12RootSignature* PMDrootsignature = nullptr;
	ID3D12DescriptorHeap* PMDDescHeap;
	ID3D12DescriptorHeap* PMDdsvHeap = nullptr;
	ID3D12DescriptorHeap* materialDescHeap = nullptr;
	std::vector<Material>materials;

	char sigunature[3] = {};
	FILE* fp;
	PMDHeader pmdheader;
	const size_t pmdvertex_size = 38;
	unsigned int vertNum;
	ID3D12Resource* vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	unsigned char* vertMap;

	D3D12_INDEX_BUFFER_VIEW ibView{};
	unsigned int indicesNum;
	ID3D12Resource* indicesBuff;


private:
	struct MatrocesDate
	{
		XMMATRIX world;
		XMMATRIX viewproj;
	};
	XMMATRIX matWorld = XMMatrixIdentity();


	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };

	XMMATRIX matScale;
	XMMATRIX matRot;
	XMMATRIX matTrans;

	XMMATRIX matView;
	ID3D12Resource* constBuff = nullptr;
	XMMATRIX matProjection;
	MatrocesDate* constMap;
	float angle = 0;
	std::string GetTexturePathFtomModelAndTexPath(
		const std::string& modelpath, const char* texpath);
	ID3D12Resource* LoadTexTureFromFile(std::string& texpath, ID3D12Device* dev);


	///////////////////
	XMFLOAT3 grav = XMFLOAT3(0, -0.1, 0);
	XMFLOAT3 velocity = XMFLOAT3(0, 0, 0);
	bool rideFlag = false;
	Input* input;
};


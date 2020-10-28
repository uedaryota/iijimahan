#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
using namespace DirectX;
class ObjFile
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;

	};
	struct ConstBufferDataB0
	{
		XMMATRIX world;
		XMMATRIX viewproj;
	};
	struct Material
	{
		std::string name;
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;
		float alpha;
		std::string textureFilename;

		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

public:
	ObjFile();
	void Initialize(ID3D12Device* dev);
	void Update();
	void Draw(ID3D12GraphicsCommandList * cmdList);
	void CreatePipeline();
	void CreateMainHeap();
	void LoadObj(const std::string name);
	void LoadMaterial(const std::string&directorypath, const std::string& filename);
	bool LoadTexture(const std::string&directorypath, const std::string& filename);

	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 scale = { 100.0f,100.0f,100.0f };

private:

	ID3D12Device* dev;

	ID3D12PipelineState* pipelinestate = nullptr;
	ID3D12RootSignature* rootsignature = nullptr;
	ID3D12DescriptorHeap* mainDescHeap;

	 std::vector<Vertex>vertices;
	 std::vector<unsigned short>indices;

	D3D12_VERTEX_BUFFER_VIEW vbView {};

	D3D12_INDEX_BUFFER_VIEW ibView{};
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;

	ComPtr<ID3D12Resource> texbuff;

	
	XMMATRIX matWorld = XMMatrixIdentity();
	XMMATRIX matView;

	XMMATRIX matProjection;
	XMMATRIX matScale;
	XMMATRIX matRot;
	XMMATRIX matTrans;
	//ワールド行列マップ
	ConstBufferDataB0* constMap0;
	//Objデータ定数マップ
	ConstBufferDataB1* constMap1;

	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffB1; // 定数バッファ

	Material material;

	D3D12_CPU_DESCRIPTOR_HANDLE CsrvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE CcbvHandle0;
	D3D12_CPU_DESCRIPTOR_HANDLE CcbvHandle1;

	D3D12_GPU_DESCRIPTOR_HANDLE GsrvHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE GcbvHandle0;
	D3D12_GPU_DESCRIPTOR_HANDLE GcbvHandle1;




};


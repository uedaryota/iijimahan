#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<vector>
using namespace std;
using namespace DirectX;
class ObjFile
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
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
		int indicesCount;
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
	~ObjFile();
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList * cmdList);
	void CreatePipeline();
	void CreateMainHeap();
	void CreateMaterialHeap();
	void SetPos(XMFLOAT3 pos);
	void SetRotate(XMFLOAT3 rotate);
	void SetScale(XMFLOAT3 scale);

	void LoadObj(const std::string name);
	void LoadMaterial(const std::string&directorypath, const std::string& filename);
	bool LoadTexture(const std::string&directorypath, const std::string& filename);
	void SetObjData(const std::string name);
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 scale = { 10.0f,10.0f,10.0f };

private:

	ID3D12Device* dev;

	ID3D12PipelineState* pipelinestate;
	ID3D12RootSignature* rootsignature;
	ID3D12DescriptorHeap* mainDescHeap;
	ID3D12DescriptorHeap* materialDescHeap;

//	ID3D12DescriptorHeap* subDescHeap;
	std::vector<Vertex>vertices;
	std::vector<unsigned short>indices;
	Vertex* vertMap;
	unsigned short* indexMap;
	D3D12_VERTEX_BUFFER_VIEW vbView {};

	D3D12_INDEX_BUFFER_VIEW ibView{};
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;

	vector<ComPtr<ID3D12Resource>> texbuffs;

	
	XMMATRIX matWorld = XMMatrixIdentity();

	XMMATRIX matScale;
	XMMATRIX matRot;
	XMMATRIX matTrans;
	//���[���h�s��}�b�v
	ConstBufferDataB0* constMap0;
	

	//Obj�f�[�^�萔�}�b�v
	vector<ConstBufferDataB1*> materialMaps;

	ComPtr<ID3D12Resource> constBuffB0;
	
	// �萔�o�b�t�@
	vector<ComPtr<ID3D12Resource>> constBuffB1; // �萔�o�b�t�@

	vector<Material> materialsDate;
	vector<Material> usematerials;

	D3D12_CPU_DESCRIPTOR_HANDLE CsrvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE CcbvHandle0;
	vector<D3D12_CPU_DESCRIPTOR_HANDLE> CmatrialHandles;

	D3D12_GPU_DESCRIPTOR_HANDLE GsrvHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE GcbvHandle0;
	vector<D3D12_GPU_DESCRIPTOR_HANDLE> GmaterialHandles;




};


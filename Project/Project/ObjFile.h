#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
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
	void CreateSubHeap();

	void LoadObj(const std::string name);
	void LoadMaterial(const std::string&directorypath, const std::string& filename);
	bool LoadTexture(const std::string&directorypath, const std::string& filename);

	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 scale = { 10.0f,10.0f,10.0f };

private:

	ID3D12Device* dev;

	ID3D12PipelineState* pipelinestate = nullptr;
	ID3D12RootSignature* rootsignature = nullptr;
	ID3D12DescriptorHeap* mainDescHeap;
	ID3D12DescriptorHeap* subDescHeap;
	std::vector<Vertex>vertices;
	 std::vector<unsigned short>indices;

	D3D12_VERTEX_BUFFER_VIEW vbView {};

	D3D12_INDEX_BUFFER_VIEW ibView{};
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;

	ComPtr<ID3D12Resource> texbuff;

	
	XMMATRIX matWorld = XMMatrixIdentity();
	XMMATRIX matView;

	XMMATRIX matProjection;
	XMMATRIX matScale;
	XMMATRIX matRot;
	XMMATRIX matTrans;
	//���[���h�s��}�b�v
	ConstBufferDataB0* constMap0;
	ConstBufferDataB0* subconstMap0;

	//Obj�f�[�^�萔�}�b�v
	ConstBufferDataB1* constMap1;

	ComPtr<ID3D12Resource> constBuffB0;
	ComPtr<ID3D12Resource> subconstBuffB0;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB1; // �萔�o�b�t�@

	Material material;

	D3D12_CPU_DESCRIPTOR_HANDLE CsrvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE CcbvHandle0;
	D3D12_CPU_DESCRIPTOR_HANDLE CcbvHandle1;

	D3D12_GPU_DESCRIPTOR_HANDLE GsrvHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE GcbvHandle0;
	D3D12_GPU_DESCRIPTOR_HANDLE GcbvHandle1;




};


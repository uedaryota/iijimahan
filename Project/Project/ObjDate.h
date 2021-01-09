#pragma once
#include<DirectXMath.h>
#include<string>
#include<vector>
#include <d3d12.h>
#include <d3dx12.h>
#include<map>
#include"ObjFile.h"
using namespace DirectX;
class ObjDate
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	struct SendData
	{
		std::vector<ObjFile::Material> materialsDate;
		std::vector<ObjFile::Material> usematerials;
		std::vector<ObjFile::ConstBufferDataB1*> materialMaps;
		// 定数バッファ
		std::vector<ComPtr<ID3D12Resource>> constBuffB1; // 定数マテリアルバッファ

		std::vector<ObjFile::Vertex>vertices;
		std::vector<unsigned short>indices;
		ObjFile::Vertex* vertMap;
		unsigned short* indexMap;
		D3D12_VERTEX_BUFFER_VIEW vbView{};

		D3D12_INDEX_BUFFER_VIEW ibView{};
		ComPtr<ID3D12Resource> vertBuff;
		// インデックスバッファ
		ComPtr<ID3D12Resource> indexBuff;
		std::vector<ComPtr<ID3D12Resource>> texbuffs;
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> CmatrialHandles;
		std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> GmaterialHandles;
		ID3D12DescriptorHeap* materialDescHeap;
	};
	ObjDate();
	~ObjDate();
	void LoadObj(const std::string name);
	void LoadMaterial(const std::string&directorypath, const std::string& filename);
	void CreateMaterialHeap();
	SendData send;
	static std::map<std::string, SendData>ObjDataList;
};


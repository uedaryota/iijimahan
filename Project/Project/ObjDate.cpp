#include "ObjDate.h"
#include<fstream>
#include<sstream>
#include"DirectXDevice.h"
using namespace std;
using namespace DirectX;
map<std::string, ObjDate::SendData>ObjDate::ObjDataList;
ObjDate::ObjDate()
{
}

ObjDate::~ObjDate()
{
}

void ObjDate::LoadObj(const std::string name)
{
	std::ifstream file;
	const string modelname = name;
	const string filename = modelname + ".obj";
	const string directorypath = "Resources/" + modelname + "/";
	const string Texdirectorypath = "Resources/";

	file.open(directorypath + filename);
	if (file.fail())
	{
		assert(0);
	}
	vector<XMFLOAT3>positions;
	vector<XMFLOAT3>normals;
	vector<XMFLOAT2>texcoords;
	send.vertices.clear();
	send.usematerials.clear();
	send.materialsDate.clear();
	string line;
	ObjFile::Material material;
	int materialNum = 0;
	//int send.indicesCount = 0;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');
		if (key == "v")
		{
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			positions.emplace_back(position);


		}
		if (key == "f")
		{
			string index_string;
			int count = 0;
			while (getline(line_stream, index_string, ' '))
			{
				send.usematerials[materialNum - 1].indicesCount += 1;
				++count;
				line_stream;
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexTexcoord, indexNormal;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				ObjFile::Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				send.vertices.emplace_back(vertex);
				send.indices.emplace_back(send.indices.size());

				if (count == 4)
				{
					send.usematerials[materialNum - 1].indicesCount += 2;
					vertex.pos = send.vertices[send.vertices.size() - 2].pos;
					vertex.normal = send.vertices[send.vertices.size() - 2].normal;
					vertex.uv = send.vertices[send.vertices.size() - 2].uv;
					send.vertices.emplace_back(vertex);
					send.indices.emplace_back(send.indices.size());

					//直前にインデックスが増えているのでー５
					vertex.pos = send.vertices[send.vertices.size() - 5].pos;
					vertex.normal = send.vertices[send.vertices.size() - 5].normal;
					vertex.uv = send.vertices[send.vertices.size() - 5].uv;
					send.vertices.emplace_back(vertex);
					send.indices.emplace_back(send.indices.size());
				}
			}


		}
		if (key == "vt")
		{
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;

			texcoords.emplace_back(texcoord);

		}
		if (key == "vn")
		{
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;


			normals.emplace_back(normal);

		}
		if (key == "usemtl")
		{
			getline(line_stream, key, ' ');
			for (int a = 0; a < send.materialsDate.size(); a++)
			{
				if (key == send.materialsDate[a].name)
				{
					material = send.materialsDate[a];
				}
			}
			send.usematerials.emplace_back(material);
			send.usematerials[materialNum].indicesCount = 0;
			materialNum += 1;
		}
		if (key == "mtllib")
		{
			string filename;
			line_stream >> filename;
			LoadMaterial(directorypath, filename);
		}
	}


	HRESULT result = S_FALSE;


	UINT sizeVB = static_cast<UINT>(sizeof(ObjFile::Vertex)*send.vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)*send.indices.size());


	// 頂点バッファ生成


	result = DirectXDevice::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&send.vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// インデックスバッファ生成
	result = DirectXDevice::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&send.indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// 頂点バッファへのデータ転送
	send.vertMap = nullptr;
	result = send.vertBuff->Map(0, nullptr, (void**)&send.vertMap);
	if (SUCCEEDED(result)) {
		//	memcpy(send.vertMap, send.vertices, sizeof(send.vertices));
		for (int a = 0; a < send.vertices.size(); a++)
		{
			send.vertMap[a] = send.vertices[a];
		}
	//	std::copy(send.vertices.begin(), send.vertices.end(), send.vertMap);
		send.vertBuff->Unmap(0, nullptr);
	}

	// インデックスバッファへのデータ転送
	send.indexMap = nullptr;
	result = send.indexBuff->Map(0, nullptr, (void**)&send.indexMap);
	if (SUCCEEDED(result)) {

		// 全インデックスに対して
		//for (int i = 0; i < _countof(send.indices); i++)
		//{
		//	send.indexMap[i] = send.indices[i];	// インデックスをコピー
		//}
		std::copy(send.indices.begin(), send.indices.end(), send.indexMap);

		send.indexBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	send.vbView.BufferLocation = send.vertBuff->GetGPUVirtualAddress();
	send.vbView.SizeInBytes = sizeVB;
	send.vbView.StrideInBytes = sizeof(send.vertices[0]);

	// インデックスバッファビューの作成
	


	send.constBuffB1.resize(send.usematerials.size());
	for (int a = 0; a < send.usematerials.size(); a++)
	{
		result = DirectXDevice::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(((sizeof(ObjFile::ConstBufferDataB1) + 0xff)&~0xff)),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&send.constBuffB1[a])
		);

	}

	//定数バッファビュー生成
	CreateMaterialHeap();

	send.materialMaps.clear();
	send.materialMaps.resize(send.usematerials.size());
	send.texbuffs.clear();
	send.texbuffs.resize(send.usematerials.size());
	for (int a = 0; a < send.usematerials.size() * 2; a += 2)
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc1 = {};
		cbvDesc1.BufferLocation = send.constBuffB1[a / 2]->GetGPUVirtualAddress();
		cbvDesc1.SizeInBytes = send.constBuffB1[a / 2]->GetDesc().Width;
		DirectXDevice::dev->CreateConstantBufferView(&cbvDesc1, send.CmatrialHandles[a]);

		send.materialMaps[a / 2] = nullptr;
		result = send.constBuffB1[a / 2]->Map(0, nullptr, (void**)&send.materialMaps[a / 2]);
		send.materialMaps[a / 2]->ambient = send.usematerials[a / 2].ambient;
		send.materialMaps[a / 2]->diffuse = send.usematerials[a / 2].diffuse;
		send.materialMaps[a / 2]->specular = send.usematerials[a / 2].specular;
		send.materialMaps[a / 2]->alpha = send.usematerials[a / 2].alpha;

		send.constBuffB1[a / 2]->Unmap(0, nullptr);
		///////////////////////
		result = S_FALSE;
		string filepath = directorypath + send.usematerials[a / 2].textureFilename;
		wchar_t wfilepath[128];
		if (send.usematerials[a / 2].textureFilename == "whitetex.png")
		{
			filepath = Texdirectorypath + send.usematerials[a / 2].textureFilename;
			int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));
			Texdirectorypath + send.usematerials[a / 2].textureFilename;

		}
		else
		{
			filepath = directorypath + send.usematerials[a / 2].textureFilename;
			int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));
			directorypath + send.usematerials[a / 2].textureFilename;
		}

		// WICテクスチャのロード
		TexMetadata metadata{};
		ScratchImage scratchImg{};

		result = LoadFromWICFile(
			wfilepath, WIC_FLAGS_NONE,
			&metadata, scratchImg);

		const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

		// リソース設定
		CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels
		);

		// テクスチャ用バッファの生成
		result = DirectXDevice::dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
			nullptr,
			IID_PPV_ARGS(&send.texbuffs[a / 2]));

		// テクスチャバッファにデータ転送
		result = send.texbuffs[a / 2]->WriteToSubresource(
			0,
			nullptr, // 全領域へコピー
			img->pixels,    // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		);

		// シェーダリソースビュー作成

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
		D3D12_RESOURCE_DESC resDesc = send.texbuffs[a / 2]->GetDesc();

		srvDesc.Format = resDesc.Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
		srvDesc.Texture2D.MipLevels = 1;

		DirectXDevice::dev->CreateShaderResourceView(send.texbuffs[a / 2].Get(), //ビューと関連付けるバッファ
			&srvDesc, //テクスチャ設定情報
			send.CmatrialHandles[a + 1]
		);
	}

	ObjDataList[name] = send;

}

void ObjDate::LoadMaterial(const std::string & directorypath, const std::string & filename)
{
	std::ifstream file;
	file.open(directorypath + filename);
	if (file.fail())
	{
		assert(0);
	}
	string line;
	ObjFile::Material material;
	material.textureFilename = "whitetex.png";
	material.name = "";
	while (getline(file, line))
	{

		std::istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}
		if (key == "newmtl")
		{
			if (material.name != "")
			{
				if (material.name != "whitetex.png")
				{
					//怪しい、要確認
					/*if (material.ambient.x == 0 && material.ambient.y == 0 && material.ambient.z == 0)
					{
						material.ambient.x = 0.1f;
						material.ambient.y = 0.1f;
						material.ambient.z = 0.1f;
					}*/
					if (material.diffuse.x == 0 && material.diffuse.y == 0 && material.diffuse.z == 0)
					{
						material.diffuse.x = 1;
						material.diffuse.y = 1;
						material.diffuse.z = 1;
					}
				}
				//一回目でなければ名前に何か入る
				send.materialsDate.emplace_back(material);
				material.textureFilename = "whitetex.png";
			}
			line_stream >> material.name;
		}
		if (key == "Ka")
		{
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}
		if (key == "Kd")
		{
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}
		if (key == "Ks")
		{
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}

		if (key == "map_Kd")
		{
			line_stream >> material.textureFilename;
			//LoadTexture(directorypath, material.textureFilename);

		}
	}
	send.materialsDate.emplace_back(material);
	file.close();
}

void ObjDate::CreateMaterialHeap()
{
	HRESULT result;
	send.materialDescHeap = nullptr;
	send.CmatrialHandles.clear();
	send.GmaterialHandles.clear();
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};

	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = send.usematerials.size() * 2;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = DirectXDevice::dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&send.materialDescHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE HeapHandle = send.materialDescHeap->GetCPUDescriptorHandleForHeapStart();

	for (int a = 0; a < send.usematerials.size(); a++)
	{
		send.CmatrialHandles.emplace_back(HeapHandle);
		HeapHandle.ptr += DirectXDevice::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		send.CmatrialHandles.emplace_back(HeapHandle);
		HeapHandle.ptr += DirectXDevice::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	}

	D3D12_GPU_DESCRIPTOR_HANDLE handle = send.materialDescHeap->GetGPUDescriptorHandleForHeapStart();
	//handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (int a = 0; a < send.usematerials.size(); a++)
	{
		send.GmaterialHandles.emplace_back(handle);
		handle.ptr += DirectXDevice::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		send.GmaterialHandles.emplace_back(handle);
		handle.ptr += DirectXDevice::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	}
}

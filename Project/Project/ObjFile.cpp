#include "ObjFile.h"
#include <d3dcompiler.h>
#include"Camera.h"
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include"DirectXDevice.h"

ObjFile::Material ObjFile::material;
Light* ObjFile::light = nullptr;

#include"ObjDate.h"
ObjFile::ObjFile()
{

}

ObjFile::~ObjFile()
{

	dev->Release();


	
	//HRESULT result = dev->Release();
		
	HRESULT result;

	if (pipelinestate);
	{
		result = pipelinestate->Release();
		pipelinestate = nullptr;
	}

	if (rootsignature);
	{
		result = rootsignature->Release();
		rootsignature = nullptr;
	}

	if (mainDescHeap);
	{
		result = mainDescHeap->Release();
		mainDescHeap = nullptr;
	}
	result = constBuffB0.Get()->Release();
	//result = constBuffB1->Release();
	result = vertBuff->Release();
	result = indexBuff->Release();
	for (int a = 0; a < texbuffs.size(); a++)
	{
		//	subDescHeap->Release();
	}

	//if (constMap0 != nullptr);
	//{
	//	delete &constMap0->viewproj;
	//	delete &constMap0->world;
		result = texbuffs[a]->Release();

	}
	texbuffs.clear();
}

void ObjFile::Initialize()
{

	this->dev = DirectXDevice::dev;
	CreatePipeline();
	CreateMainHeap();
}

void ObjFile::Update()
{
	constMap0 = nullptr;
	HRESULT result = constBuffB0->Map(0, nullptr, (void**)&constMap0);


	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matRot *= XMMatrixRotationZ(rotation.z);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	constMap0->world = matWorld;
	constMap0->viewproj = matView * matProjection;

	constBuffB0->Unmap(0, nullptr);

	constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;


	constMap0->viewproj = Camera::ReturnCameraState()->matView * Camera::ReturnCameraState()->matProjection;
	
	constBuffB0->Unmap(0, nullptr);

	//constMap1 = nullptr;
	//result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	//constMap1->ambient = material.ambient;
	//constMap1->diffuse = material.diffuse;
	//constMap1->specular = material.specular;
	//constMap1->alpha = material.alpha;

	//constBuffB1->Unmap(0, nullptr);
}

void ObjFile::Draw(ID3D12GraphicsCommandList * cmdList)
{
	HRESULT result = constBuffB0->Map(0, nullptr, (void**)&constMap0);

	constMap0->world = matWorld;
	constMap0->viewproj = Camera::ReturnCameraState()->matView *  Camera::ReturnCameraState()->matProjection;

	constBuffB0->Unmap(0, nullptr);



	D3D12_GPU_DESCRIPTOR_HANDLE handle = mainDescHeap->GetGPUDescriptorHandleForHeapStart();
	GsrvHandle = handle;
	handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	GcbvHandle0 = handle;
	handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	GcbvHandle1 = handle;
	
	cmdList->SetPipelineState(pipelinestate);
	cmdList->SetGraphicsRootSignature(rootsignature);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->IASetIndexBuffer(&ibView);
	cmdList->SetDescriptorHeaps(1, &mainDescHeap);
	cmdList->SetGraphicsRootDescriptorTable(0, GsrvHandle);
	cmdList->SetGraphicsRootDescriptorTable(1, GcbvHandle0);
	cmdList->SetGraphicsRootDescriptorTable(2, GcbvHandle1);

	//ライトの描画
	light->Draw(cmdList, 3);

	cmdList->DrawInstanced((UINT)vertices.size(), 1, 0, 0);

	cmdList->SetDescriptorHeaps(1, &materialDescHeap);
	//cmdList->SetGraphicsRootDescriptorTable(2, GmaterialHandles[0]);
	//cmdList->DrawInstanced((UINT)vertices.size(), 1, 0, 0);
	UINT start = 0;
	for (int a = 0; a < usematerials.size() * 2; a += 2)
	{
		cmdList->SetGraphicsRootDescriptorTable(0, GmaterialHandles[a + 1]);
		cmdList->SetGraphicsRootDescriptorTable(2, GmaterialHandles[a]);
		
		cmdList->DrawInstanced((UINT)usematerials[a / 2].indicesCount, 1, start, 0);
		start += usematerials[a / 2].indicesCount;
	}
	
}

void ObjFile::CreatePipeline()
{

	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT result;
	result = D3DCompileFromFile(
		L"ObjVertexShader.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"ObjPixelShader.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);


	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//xyz座標
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
		},
			{ // 法線
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
{//uv座標
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
		},
	};








	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};


	gpipeline.pRootSignature = nullptr;
	gpipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = psBlob->GetBufferSize();

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	gpipeline.RasterizerState.MultisampleEnable = false;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	gpipeline.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	gpipeline.BlendState.AlphaToCoverageEnable = false;
	gpipeline.BlendState.IndependentBlendEnable = false;
	// レンダーターゲットのブレンド設定(8 個あるがいまは一つしか使わない) 
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // 標準設定
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;  // 加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;  // ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // 1.0f-ソースのアルファ値 

	// ブレンドステートに設定する
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // 描画対象は 1 つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0~255 指定の RGBA
	gpipeline.SampleDesc.Count = 1; // 1 ピクセルにつき 1 回サンプリング
	gpipeline.SampleDesc.Quality = 0;

	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.DepthStencilState.StencilEnable = false;

	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;



	D3D12_ROOT_SIGNATURE_DESC rootSigunatureDesc = {};
	rootSigunatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	///////////////////////////////




	D3D12_DESCRIPTOR_RANGE descTblRange[3] = {};
	descTblRange[0].NumDescriptors = 1;
	descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//シェーダリソースタイプ
	descTblRange[0].BaseShaderRegister = 0;//t0に送る
	descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descTblRange[1].NumDescriptors = 1;
	descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;//定数タイプ
	descTblRange[1].BaseShaderRegister = 0;//b0に送る
	descTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descTblRange[2].NumDescriptors = 1;
	descTblRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;//定数タイプ
	descTblRange[2].BaseShaderRegister = 1;//b1に送る
	descTblRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//descTblRange[3].NumDescriptors = 1;
	//descTblRange[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;//定数タイプ
	//descTblRange[3].BaseShaderRegister = 2;//b2に送る
	//descTblRange[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	D3D12_ROOT_PARAMETER rootparam[4] = {};

	rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ALL
	rootparam[0].DescriptorTable.pDescriptorRanges = &descTblRange[0];
	rootparam[0].DescriptorTable.NumDescriptorRanges = 1;

	rootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//ALL
	rootparam[1].DescriptorTable.pDescriptorRanges = &descTblRange[1];
	rootparam[1].DescriptorTable.NumDescriptorRanges = 1;

	rootparam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ALL
	rootparam[2].DescriptorTable.pDescriptorRanges = &descTblRange[2];
	rootparam[2].DescriptorTable.NumDescriptorRanges = 1;

	rootparam[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootparam[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ALL
	rootparam[3].Constants.ShaderRegister = 2;
	rootparam[3].Constants.RegisterSpace = 0;
	//rootparam[3].DescriptorTable.pDescriptorRanges = &descTblRange[3];
	//rootparam[3].DescriptorTable.NumDescriptorRanges = 1;

	rootSigunatureDesc.pParameters = rootparam;
	rootSigunatureDesc.NumParameters = _countof(rootparam);

	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};

	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.MaxAnisotropy = 16;
	rootSigunatureDesc.pStaticSamplers = &samplerDesc;
	rootSigunatureDesc.NumStaticSamplers = 1;
	///////////////////////////////



	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(
		&rootSigunatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);

	result = dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootsignature)
	);


	gpipeline.pRootSignature = rootsignature;

	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));


}

void ObjFile::CreateMainHeap()
{
	HRESULT result;
	mainDescHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};

	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 2;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&mainDescHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE HeapHandle = mainDescHeap->GetCPUDescriptorHandleForHeapStart();
	CsrvHandle = HeapHandle;
	HeapHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CcbvHandle0 = HeapHandle;


	D3D12_GPU_DESCRIPTOR_HANDLE handle = mainDescHeap->GetGPUDescriptorHandleForHeapStart();
	GsrvHandle = handle;
	handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	GcbvHandle0 = handle;

}

void ObjFile::CreateMaterialHeap()
{
	//HRESULT result;
	//materialDescHeap = nullptr;
	//CmatrialHandles.clear();
	//GmaterialHandles.clear();
	//D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	//
	//descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//descHeapDesc.NodeMask = 0;
	//descHeapDesc.NumDescriptors = usematerials.size() * 2;
	//descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&materialDescHeap));

	//D3D12_CPU_DESCRIPTOR_HANDLE HeapHandle = materialDescHeap->GetCPUDescriptorHandleForHeapStart();

	//for (int a = 0; a < usematerials.size(); a++)
	//{
	//	CmatrialHandles.emplace_back(HeapHandle);
	//	HeapHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//	CmatrialHandles.emplace_back(HeapHandle);
	//	HeapHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//}

	//D3D12_GPU_DESCRIPTOR_HANDLE handle = materialDescHeap->GetGPUDescriptorHandleForHeapStart();
	////handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//for (int a = 0; a < usematerials.size(); a++)
	//{
	//	GmaterialHandles.emplace_back(handle);
	//	handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//	GmaterialHandles.emplace_back(handle);
	//	handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//}
}



void ObjFile::SetPos(XMFLOAT3 pos)
{
	position = pos;
}

void ObjFile::SetRotate(XMFLOAT3 rotate)
{
	rotation = rotate;
}

void ObjFile::SetScale(XMFLOAT3 scale)
{
	this->scale = scale;
}

void ObjFile::LoadObj(std::string name)
{
	std::ifstream file;
	const string modelname = name;
	const string filename = modelname + ".obj";
	const string directorypath = "Resources/" + modelname + "/";
	file.open(directorypath + filename);
	if (file.fail())
	{
		assert(0);
	}
	vector<XMFLOAT3>positions;
	vector<XMFLOAT3>normals;
	vector<XMFLOAT2>texcoords;

	string line;
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
				++count;
				line_stream;
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexTexcoord, indexNormal;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);
				indices.emplace_back(indices.size());

				if (count == 4)
				{
					vertex.pos = vertices[vertices.size() - 2].pos;
					vertex.normal = vertices[vertices.size() - 2].normal;
					vertex.uv = vertices[vertices.size() - 2].uv;
					vertices.emplace_back(vertex);
					indices.emplace_back(indices.size());

					//直前にインデックスが増えているのでー５
					vertex.pos = vertices[vertices.size() - 5].pos;
					vertex.normal = vertices[vertices.size() - 5].normal;
					vertex.uv = vertices[vertices.size() - 5].uv;
					vertices.emplace_back(vertex);
					indices.emplace_back(indices.size());
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
		if (key == "mtllib")
		{
			string filename;
			line_stream >> filename;
			LoadMaterial(directorypath, filename);
		}
	}


	HRESULT result = S_FALSE;


	UINT sizeVB = static_cast<UINT>(sizeof(Vertex)*vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)*indices.size());


	// 頂点バッファ生成


	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// インデックスバッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// 頂点バッファへのデータ転送
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		//	memcpy(vertMap, vertices, sizeof(vertices));
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {

		// 全インデックスに対して
		//for (int i = 0; i < _countof(indices); i++)
		//{
		//	indexMap[i] = indices[i];	// インデックスをコピー
		//}
		std::copy(indices.begin(), indices.end(), indexMap);

		indexBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	matWorld.r[0].m128_f32[0] = 2.0f / Camera::window_width;
	matWorld.r[1].m128_f32[1] = -2.0f / Camera::window_height;
	matWorld.r[3].m128_f32[0] = -1.0f;
	matWorld.r[3].m128_f32[1] = 1.0f;

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matRot *= XMMatrixRotationZ(rotation.z);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	matWorld = XMMatrixIdentity();
	matWorld.r[0].m128_f32[0] = 2.0f / Camera::window_width;
	matWorld.r[1].m128_f32[1] = -2.0f / Camera::window_height;
	matWorld.r[3].m128_f32[0] = -1.0f;
	matWorld.r[3].m128_f32[1] = 1.0f;
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;


	HRESULT result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0)
	);

	//定数バッファビュー生成

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc0 = {};
	cbvDesc0.BufferLocation = constBuffB0->GetGPUVirtualAddress();
	cbvDesc0.SizeInBytes = constBuffB0->GetDesc().Width;
	dev->CreateConstantBufferView(&cbvDesc0, CcbvHandle0);

	constMap0 = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap0);

	constMap0->world = matWorld;
	constMap0->viewproj = Camera::ReturnCameraState()->matView * Camera::ReturnCameraState()->matProjection;

	constBuffB0->Unmap(0, nullptr);

	SetObjData(name);

}
//void ObjFile::LoadObj(std::string name)
//{
//	std::ifstream file;
//	const string modelname = name;
//	const string filename = modelname + ".obj";
//	const string directorypath = "Resources/" + modelname + "/";
//	const string Texdirectorypath = "Resources/";
//
//	file.open(directorypath + filename);
//	if (file.fail())
//	{
//		assert(0);
//	}
//	vector<XMFLOAT3>positions;
//	vector<XMFLOAT3>normals;
//	vector<XMFLOAT2>texcoords;
//	vertices.clear();
//	usematerials.clear();
//	materialsDate.clear();
//	string line;
//	Material material;
//	int materialNum = 0;
//	//int indicesCount = 0;
//	while (getline(file, line))
//	{
//		std::istringstream line_stream(line);
//		string key;
//		getline(line_stream, key, ' ');
//		if (key == "v")
//		{
//			XMFLOAT3 position{};
//			line_stream >> position.x;
//			line_stream >> position.y;
//			line_stream >> position.z;
//
//			positions.emplace_back(position);
//
//
//		}
//		if (key == "f")
//		{
//			string index_string;
//			int count = 0;
//			while (getline(line_stream, index_string, ' '))
//			{
//				usematerials[materialNum - 1].indicesCount += 1;
//				++count;
//				line_stream;
//				std::istringstream index_stream(index_string);
//				unsigned short indexPosition, indexTexcoord, indexNormal;
//				index_stream >> indexPosition;
//				index_stream.seekg(1, ios_base::cur);
//				index_stream >> indexTexcoord;
//				index_stream.seekg(1, ios_base::cur);
//				index_stream >> indexNormal;
//
//				Vertex vertex{};
//				vertex.pos = positions[indexPosition - 1];
//				vertex.normal = normals[indexNormal - 1];
//				vertex.uv = texcoords[indexTexcoord - 1];
//				vertices.emplace_back(vertex);
//				indices.emplace_back(indices.size());
//
//				if (count == 4)
//				{
//					usematerials[materialNum - 1].indicesCount += 2;
//					vertex.pos = vertices[vertices.size() - 2].pos;
//					vertex.normal = vertices[vertices.size() - 2].normal;
//					vertex.uv = vertices[vertices.size() - 2].uv;
//					vertices.emplace_back(vertex);
//					indices.emplace_back(indices.size());
//
//					//直前にインデックスが増えているのでー５
//					vertex.pos = vertices[vertices.size() - 5].pos;
//					vertex.normal = vertices[vertices.size() - 5].normal;
//					vertex.uv = vertices[vertices.size() - 5].uv;
//					vertices.emplace_back(vertex);
//					indices.emplace_back(indices.size());
//				}
//			}
//
//
//		}
//		if (key == "vt")
//		{
//			XMFLOAT2 texcoord{};
//			line_stream >> texcoord.x;
//			line_stream >> texcoord.y;
//			texcoord.y = 1.0f - texcoord.y;
//
//			texcoords.emplace_back(texcoord);
//
//		}
//		if (key == "vn")
//		{
//			XMFLOAT3 normal{};
//			line_stream >> normal.x;
//			line_stream >> normal.y;
//			line_stream >> normal.z;
//
//
//			normals.emplace_back(normal);
//
//		}
//		if (key == "usemtl")
//		{
//			getline(line_stream, key, ' ');
//			for (int a = 0; a < materialsDate.size(); a++)
//			{
//				if (key == materialsDate[a].name)
//				{
//					material = materialsDate[a];
//				}
//			}
//			usematerials.emplace_back(material);
//			usematerials[materialNum].indicesCount = 0;
//			materialNum += 1;
//		}
//		if (key == "mtllib")
//		{
//			string filename;
//			line_stream >> filename;
//			LoadMaterial(directorypath, filename);
//		}
//	}
//
//
//	HRESULT result = S_FALSE;
//
//
//	UINT sizeVB = static_cast<UINT>(sizeof(Vertex)*vertices.size());
//	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)*indices.size());
//
//
//	// 頂点バッファ生成
//
//
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertBuff));
//	if (FAILED(result)) {
//		assert(0);
//		return;
//	}
//
//	// インデックスバッファ生成
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&indexBuff));
//	if (FAILED(result)) {
//		assert(0);
//		return;
//	}
//
//	// 頂点バッファへのデータ転送
//	vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	if (SUCCEEDED(result)) {
//		//	memcpy(vertMap, vertices, sizeof(vertices));
//		std::copy(vertices.begin(), vertices.end(), vertMap);
//		vertBuff->Unmap(0, nullptr);
//	}
//
//	// インデックスバッファへのデータ転送
//	indexMap = nullptr;
//	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
//	if (SUCCEEDED(result)) {
//
//		// 全インデックスに対して
//		//for (int i = 0; i < _countof(indices); i++)
//		//{
//		//	indexMap[i] = indices[i];	// インデックスをコピー
//		//}
//		std::copy(indices.begin(), indices.end(), indexMap);
//
//		indexBuff->Unmap(0, nullptr);
//	}
//
//	// 頂点バッファビューの作成
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	vbView.SizeInBytes = sizeVB;
//	vbView.StrideInBytes = sizeof(vertices[0]);
//
//	// インデックスバッファビューの作成
//	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
//	ibView.Format = DXGI_FORMAT_R16_UINT;
//	ibView.SizeInBytes = sizeIB;
//
//	matWorld.r[0].m128_f32[0] = 2.0f / Camera::window_width;
//	matWorld.r[1].m128_f32[1] = -2.0f / Camera::window_height;
//	matWorld.r[3].m128_f32[0] = -1.0f;
//	matWorld.r[3].m128_f32[1] = 1.0f;
//
//	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
//	matRot = XMMatrixIdentity();
//	matRot *= XMMatrixRotationX(rotation.x);
//	matRot *= XMMatrixRotationY(rotation.y);
//	matRot *= XMMatrixRotationZ(rotation.z);
//	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
//	matWorld = XMMatrixIdentity();
//	matWorld.r[0].m128_f32[0] = 2.0f / Camera::window_width;
//	matWorld.r[1].m128_f32[1] = -2.0f / Camera::window_height;
//	matWorld.r[3].m128_f32[0] = -1.0f;
//	matWorld.r[3].m128_f32[1] = 1.0f;
//	matWorld *= matScale;
//	matWorld *= matRot;
//	matWorld *= matTrans;
//
//
//	result = dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff)&~0xff),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&constBuffB0)
//	);
//	constBuffB1.resize(usematerials.size());
//	for (int a = 0; a < usematerials.size(); a++)
//	{
//		result = dev->CreateCommittedResource(
//			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//			D3D12_HEAP_FLAG_NONE,
//			&CD3DX12_RESOURCE_DESC::Buffer(((sizeof(ConstBufferDataB1) + 0xff)&~0xff)),
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&constBuffB1[a])
//		);
//
//	}
//
//	//定数バッファビュー生成
//
//	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc0 = {};
//	cbvDesc0.BufferLocation = constBuffB0->GetGPUVirtualAddress();
//	cbvDesc0.SizeInBytes = constBuffB0->GetDesc().Width;
//	dev->CreateConstantBufferView(&cbvDesc0, CcbvHandle0);
//
//	constMap0 = nullptr;
//	result = constBuffB0->Map(0, nullptr, (void**)&constMap0);
//
//	constMap0->world = matWorld;
//	constMap0->viewproj = Camera::ReturnCameraState()->matView * Camera::ReturnCameraState()->matProjection;
//
//	constBuffB0->Unmap(0, nullptr);
//
//	CreateMaterialHeap();
//
//	materialMaps.clear();
//	materialMaps.resize(usematerials.size());
//	texbuffs.clear();
//	texbuffs.resize(usematerials.size());
//	for (int a = 0; a < usematerials.size() * 2; a += 2)
//	{
//		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc1 = {};
//		cbvDesc1.BufferLocation = constBuffB1[a / 2]->GetGPUVirtualAddress();
//		cbvDesc1.SizeInBytes = constBuffB1[a / 2]->GetDesc().Width;
//		dev->CreateConstantBufferView(&cbvDesc1, CmatrialHandles[a]);
//
//		materialMaps[a / 2] = nullptr;
//		result = constBuffB1[a / 2]->Map(0, nullptr, (void**)&materialMaps[a / 2]);
//		materialMaps[a / 2]->ambient = usematerials[a / 2].ambient;
//		materialMaps[a / 2]->diffuse = usematerials[a / 2].diffuse;
//		materialMaps[a / 2]->specular = usematerials[a / 2].specular;
//		materialMaps[a / 2]->alpha = usematerials[a / 2].alpha;
//
//		constBuffB1[a / 2]->Unmap(0, nullptr);
//		///////////////////////
//		result = S_FALSE;
//		string filepath = directorypath + usematerials[a / 2].textureFilename;
//		wchar_t wfilepath[128];
//		if (usematerials[a / 2].textureFilename == "whitetex.png")
//		{
//			filepath = Texdirectorypath + usematerials[a / 2].textureFilename;
//			int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));
//			Texdirectorypath + usematerials[a / 2].textureFilename;
//
//		}
//		else
//		{
//			filepath = directorypath + usematerials[a / 2].textureFilename;
//			int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));
//			directorypath + usematerials[a / 2].textureFilename;
//		}
//
//		// WICテクスチャのロード
//		TexMetadata metadata{};
//		ScratchImage scratchImg{};
//
//		result = LoadFromWICFile(
//			wfilepath, WIC_FLAGS_NONE,
//			&metadata, scratchImg);
//
//		const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出
//
//		// リソース設定
//		CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
//			metadata.format,
//			metadata.width,
//			(UINT)metadata.height,
//			(UINT16)metadata.arraySize,
//			(UINT16)metadata.mipLevels
//		);
//
//		// テクスチャ用バッファの生成
//		result = dev->CreateCommittedResource(
//			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
//			D3D12_HEAP_FLAG_NONE,
//			&texresDesc,
//			D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
//			nullptr,
//			IID_PPV_ARGS(&texbuffs[a / 2]));
//
//		// テクスチャバッファにデータ転送
//		result = texbuffs[a / 2]->WriteToSubresource(
//			0,
//			nullptr, // 全領域へコピー
//			img->pixels,    // 元データアドレス
//			(UINT)img->rowPitch,  // 1ラインサイズ
//			(UINT)img->slicePitch // 1枚サイズ
//		);
//
//		// シェーダリソースビュー作成
//
//		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
//		D3D12_RESOURCE_DESC resDesc = texbuffs[a / 2]->GetDesc();
//
//		srvDesc.Format = resDesc.Format;
//		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
//		srvDesc.Texture2D.MipLevels = 1;
//
//		dev->CreateShaderResourceView(texbuffs[a / 2].Get(), //ビューと関連付けるバッファ
//			&srvDesc, //テクスチャ設定情報
//			CmatrialHandles[a + 1]
//		);
//	}
//
//
//}

void ObjFile::LoadMaterial(const std::string & directorypath, const std::string & filename)
{
	std::ifstream file;
	file.open(directorypath + filename);
	if (file.fail())
	{
		assert(0);
	}
	string line;
	Material material;
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
				materialsDate.emplace_back(material);
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
	materialsDate.emplace_back(material);
	file.close();
}

bool ObjFile::LoadTexture(const std::string&directorypath, const std::string& filename)
{

	HRESULT result = S_FALSE;

	string filepath = directorypath + filename;
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));
	directorypath + filename;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		return result;
	}
	//
	//HRESULT result = S_FALSE;

	//string filepath = directorypath + filename;
	//wchar_t wfilepath[128];
	//int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));
	//directorypath + filename;

	//// WICテクスチャのロード
	//TexMetadata metadata{};
	//ScratchImage scratchImg{};

	//result = LoadFromWICFile(
	//	wfilepath, WIC_FLAGS_NONE,
	//	&metadata, scratchImg);
	//if (FAILED(result)) {
	//	return result;
	//}

	//const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	//// リソース設定
	//CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	//	metadata.format,
	//	metadata.width,
	//	(UINT)metadata.height,
	//	(UINT16)metadata.arraySize,
	//	(UINT16)metadata.mipLevels
	//);

	//// テクスチャ用バッファの生成
	//result = dev->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
	//	D3D12_HEAP_FLAG_NONE,
	//	&texresDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
	//	nullptr,
	//	IID_PPV_ARGS(&texbuff));
	//if (FAILED(result)) {
	//	return result;
	//}

	//// テクスチャバッファにデータ転送
	//result = texbuff->WriteToSubresource(
	//	0,
	//	nullptr, // 全領域へコピー
	//	img->pixels,    // 元データアドレス
	//	(UINT)img->rowPitch,  // 1ラインサイズ
	//	(UINT)img->slicePitch // 1枚サイズ
	//);
	//if (FAILED(result)) {
	//	return result;
	//}

	//// シェーダリソースビュー作成
	//
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	//D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	//srvDesc.Format = resDesc.Format;
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	//srvDesc.Texture2D.MipLevels = 1;

	// シェーダリソースビュー作成

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();
	//dev->CreateShaderResourceView(texbuff.Get(), //ビューと関連付けるバッファ
	//	&srvDesc, //テクスチャ設定情報
	//	CsrvHandle
	//);

	return true;
}

void ObjFile::SetObjData(const std::string name)
{

	std::copy(ObjDate::ObjDataList[name].CmatrialHandles.begin(),
		ObjDate::ObjDataList[name].CmatrialHandles.end(), std::back_inserter(CmatrialHandles));
	constBuffB1= ObjDate::ObjDataList[name].constBuffB1;
	std::copy(ObjDate::ObjDataList[name].GmaterialHandles.begin(),
		ObjDate::ObjDataList[name].GmaterialHandles.end(), std::back_inserter(GmaterialHandles));
	ibView = ObjDate::ObjDataList[name].ibView;
	indexBuff = ObjDate::ObjDataList[name].indexBuff;
	indexMap = ObjDate::ObjDataList[name].indexMap;
	indices = ObjDate::ObjDataList[name].indices;
	materialDescHeap = ObjDate::ObjDataList[name].materialDescHeap;
	std::copy(ObjDate::ObjDataList[name].materialMaps.begin(), 
		ObjDate::ObjDataList[name].materialMaps.end(), std::back_inserter(materialMaps));
	//ObjDate::ObjDataList[name].materialsDate;
	std::copy(ObjDate::ObjDataList[name].texbuffs.begin(),
		ObjDate::ObjDataList[name].texbuffs.end(), std::back_inserter(texbuffs));
	std::copy(ObjDate::ObjDataList[name].usematerials.begin(),
		ObjDate::ObjDataList[name].usematerials.end(), std::back_inserter(usematerials));
	vbView = ObjDate::ObjDataList[name].vbView;
	vertBuff = ObjDate::ObjDataList[name].vertBuff;
	std::copy(ObjDate::ObjDataList[name].vertices.begin(),
		ObjDate::ObjDataList[name].vertices.end(), std::back_inserter(vertices));
	//std::copy(vertMap, ObjDate::ObjDataList[name].vertMap);
}

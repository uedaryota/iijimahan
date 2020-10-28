#include "PMDClass.h"

#include"Input.h"
PMDClass::PMDClass()
{
}
ID3D12Resource* CreateWhiteTexture(ID3D12Device* dev);
void PMDClass::Initialize(ID3D12Device* dev, Camera* camera)
{
	c = camera;
	CreateMainHeap(dev);
	CreatePipeline(dev);
	SetDepth(dev);
	std::string strModelPath = "Model/cocoonP_2.pmd";
	fp = fopen(strModelPath.c_str(), "rb");
	fread(sigunature, sizeof(sigunature), 1, fp);
	fread(&pmdheader, sizeof(pmdheader), 1, fp);


	fread(&vertNum, sizeof(vertNum), 1, fp);

	std::vector<unsigned char>vertices(vertNum*pmdvertex_size);
	fread(vertices.data(), vertices.size(), 1, fp);

	fread(&indicesNum, sizeof(indicesNum), 1, fp);

	std::vector<unsigned short>indices;

	indices.resize(indicesNum);
	fread(indices.data(), indices.size() * sizeof(indices[0]), 1, fp);

	unsigned int materialNum;
	fread(&materialNum, sizeof(materialNum), 1, fp);
	std::vector<PMDMaterial> pmdMaterials(materialNum);
	fread(pmdMaterials.data(), pmdMaterials.size() * sizeof(PMDMaterial), 1, fp);

	fclose(fp);

	HRESULT result;
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vertices.size()),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = vertices.size();
	vbView.StrideInBytes = pmdvertex_size;

	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(vertices), std::end(vertices), vertMap);

	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(indices.size() * sizeof(indices[0])),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indicesBuff)
	);
	unsigned short* indexMap = nullptr;
	indicesBuff->Map(0, nullptr, (void**)&indexMap);
	std::copy(std::begin(indices), std::end(indices), indexMap);
	indicesBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indicesBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = indices.size() * sizeof(indices[0]);

	
	std::vector<ID3D12Resource*> textureResources;
	textureResources.resize(pmdMaterials.size());
	for (int i = 0; i < pmdMaterials.size(); ++i)
	{
		if (strlen(pmdMaterials[i].texFilePath) == 0)
		{
			textureResources[i] = nullptr;
		}
		auto texfilepath = GetTexturePathFtomModelAndTexPath(strModelPath, pmdMaterials[i].texFilePath);

		textureResources[i] = LoadTexTureFromFile(texfilepath, dev);
	}

	materials.resize(pmdMaterials.size());
	for (int i = 0; i < pmdMaterials.size(); ++i)
	{
		materials[i].indicesNum = pmdMaterials[i].indicesNum;
		materials[i].material.diffuse = pmdMaterials[i].diffuse;
		materials[i].material.alpha = pmdMaterials[i].alpha;
		materials[i].material.specular = pmdMaterials[i].specular;
		materials[i].material.speculaty = pmdMaterials[i].specularity;
		materials[i].material.ambient = pmdMaterials[i].ambient;
	}

	unsigned int materialBuffsize = sizeof(MaterialForhlsl);
	materialBuffsize = (materialBuffsize + 0xff)&~0xff;

	ID3D12Resource* materialBuff = nullptr;
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(materialBuffsize*materialNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&materialBuff)
	);

	char* mapMaterial = nullptr;
	result = materialBuff->Map(0, nullptr, (void**)&mapMaterial);

	for (auto& m : materials)
	{
		*((MaterialForhlsl*)mapMaterial) = m.material;
		mapMaterial += materialBuffsize;
	}

	materialBuff->Unmap(0, nullptr);

	materialDescHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC matDescHeapDesc{};
	matDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	matDescHeapDesc.NodeMask = 0;
	matDescHeapDesc.NumDescriptors = materialNum*2;
	matDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = dev->CreateDescriptorHeap(&matDescHeapDesc, IID_PPV_ARGS(&materialDescHeap));

	D3D12_CONSTANT_BUFFER_VIEW_DESC matCBVDesc = {};
	matCBVDesc.BufferLocation = materialBuff->GetGPUVirtualAddress();
	matCBVDesc.SizeInBytes = materialBuffsize;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	D3D12_CPU_DESCRIPTOR_HANDLE matDescHeahHandle = materialDescHeap->GetCPUDescriptorHandleForHeapStart();
	auto inc = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	ID3D12Resource* whitetex = CreateWhiteTexture(dev);
	for (int i = 0; i < materialNum; i++)
	{
		dev->CreateConstantBufferView(&matCBVDesc, matDescHeahHandle);
		matDescHeahHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		matCBVDesc.BufferLocation += materialBuffsize;

		if (textureResources[i] != nullptr)
		{
			srvDesc.Format = textureResources[i]->GetDesc().Format;
			dev->CreateShaderResourceView(
				textureResources[i],
				&srvDesc,
				matDescHeahHandle
			);
		}
		else
		{
			srvDesc.Format = whitetex->GetDesc().Format;
			dev->CreateShaderResourceView(
			    whitetex,
				&srvDesc,
				matDescHeahHandle
			);
		}

		matDescHeahHandle.ptr += inc;
	}


}

void PMDClass::CreatePipeline(ID3D12Device * dev)
{
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT result;
	result = D3DCompileFromFile(
		L"PMDVertexShader.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"PMDPixelShader.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);



	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
"POSITION",
0,
DXGI_FORMAT_R32G32B32_FLOAT,
0,
D3D12_APPEND_ALIGNED_ELEMENT,
D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
0
}, // (1 行で書いたほうが見やすい)

		{
"NORMAL",
0,
DXGI_FORMAT_R32G32B32_FLOAT,
0,
D3D12_APPEND_ALIGNED_ELEMENT,
D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
0
}, // (1 行で書いたほうが見やすい)

		{
"TEXCOORD",
0,
DXGI_FORMAT_R32G32_FLOAT,
0,
D3D12_APPEND_ALIGNED_ELEMENT,
D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
0
}, // (1 行で書いたほうが見やすい)

		{
"BONE_NO",
0,
DXGI_FORMAT_R16G16_UINT,
0,
D3D12_APPEND_ALIGNED_ELEMENT,
D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
0
}, // (1 行で書いたほうが見やすい)

		{
"WEIGHT",
0,
DXGI_FORMAT_R8_UINT,
0,
D3D12_APPEND_ALIGNED_ELEMENT,
D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
0
}, // (1 行で書いたほうが見やすい)

		{
"EDGE_FLG",
0,
DXGI_FORMAT_R8_UINT,
0,
D3D12_APPEND_ALIGNED_ELEMENT,
D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
0
}, // (1 行で書いたほうが見やすい)

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




	D3D12_DESCRIPTOR_RANGE descTblRange[4] = {};
	descTblRange[0].NumDescriptors = 1;
	descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descTblRange[0].BaseShaderRegister = 1;
	descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descTblRange[1].NumDescriptors = 1;
	descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descTblRange[1].BaseShaderRegister = 0;
	descTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descTblRange[2].NumDescriptors = 1;
	descTblRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descTblRange[2].BaseShaderRegister = 1;
	descTblRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descTblRange[3].NumDescriptors = 1;
	descTblRange[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descTblRange[3].BaseShaderRegister = 0;
	descTblRange[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	D3D12_ROOT_PARAMETER rootparam[3] = {};

	rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ALL
	rootparam[0].DescriptorTable.pDescriptorRanges = &descTblRange[0];
	rootparam[0].DescriptorTable.NumDescriptorRanges = 1;

	rootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//ALL
	rootparam[1].DescriptorTable.pDescriptorRanges = &descTblRange[1];
	rootparam[1].DescriptorTable.NumDescriptorRanges = 1;

	rootparam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ALL
	rootparam[2].DescriptorTable.pDescriptorRanges = &descTblRange[2];
	rootparam[2].DescriptorTable.NumDescriptorRanges = 2;

	rootSigunatureDesc.pParameters = rootparam;
	rootSigunatureDesc.NumParameters = 3;

	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};

	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

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
		IID_PPV_ARGS(&PMDrootsignature)
	);
	gpipeline.pRootSignature = PMDrootsignature;
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&PMDpipelinestate));


}

void PMDClass::CreateMainHeap(ID3D12Device * dev)
{
	HRESULT result;
	PMDDescHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};

	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 2;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&PMDDescHeap));



	D3D12_CPU_DESCRIPTOR_HANDLE HeapHandle = PMDDescHeap->GetCPUDescriptorHandleForHeapStart();

	
	//HeapHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	matWorld.r[0].m128_f32[0] = 2.0f / Camera::window_width;
	matWorld.r[1].m128_f32[1] = -2.0f / Camera::window_height;
	matWorld.r[3].m128_f32[0] = -1.0f;
	matWorld.r[3].m128_f32[1] = 1.0f;


	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&c->CameraPos()), XMLoadFloat3(&c->Target()), XMLoadFloat3(&c->Up())
	);


	matProjection = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast<float>(Camera::window_width) / static_cast<float>(Camera::window_height),
		1.0f,
		1000.0f
	);

	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatrocesDate) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);


	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->world = matWorld;
	constMap->viewproj = matView * matProjection;
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = constBuff->GetDesc().Width;
	dev->CreateConstantBufferView(&cbvDesc, HeapHandle);
}

void PMDClass::SetDepth(ID3D12Device * dev)
{
	D3D12_RESOURCE_DESC depthresdesc = {};
	depthresdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthresdesc.Width = Camera::window_width;
	depthresdesc.Height = Camera::window_height;
	depthresdesc.DepthOrArraySize = 1;
	depthresdesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthresdesc.SampleDesc.Count = 1;
	depthresdesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;


	D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;

	ID3D12Resource* depthBuff = nullptr;
	HRESULT result;
	result = dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthresdesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff)
	);

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&PMDdsvHeap));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	dev->CreateDepthStencilView(
		depthBuff,
		&dsvDesc,
		PMDdsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void PMDClass::Draw(ID3D12GraphicsCommandList * cmdList, ID3D12Device* dev)
{
	cmdList->SetPipelineState(PMDpipelinestate);
	cmdList->SetGraphicsRootSignature(PMDrootsignature);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->IASetIndexBuffer(&ibView);
	cmdList->SetDescriptorHeaps(1, &PMDDescHeap);
	cmdList->SetGraphicsRootDescriptorTable(1, PMDDescHeap->GetGPUDescriptorHandleForHeapStart());
	//D3D12_GPU_DESCRIPTOR_HANDLE handle = pmd->PMDDescHeap->GetGPUDescriptorHandleForHeapStart();
	//handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cmdList->SetDescriptorHeaps(1, &materialDescHeap);
	cmdList->SetGraphicsRootDescriptorTable(2, materialDescHeap->GetGPUDescriptorHandleForHeapStart());

	//cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	//cmdList->DrawInstanced(pmd->vertNum, 1, 0, 0);
	//cmdList->DrawIndexedInstanced(indicesNum, 1, 0, 0, 0);
	D3D12_GPU_DESCRIPTOR_HANDLE materialH = materialDescHeap->GetGPUDescriptorHandleForHeapStart();
	unsigned int idxOffset = 0;
	for (auto m : materials)
	{
		cmdList->SetGraphicsRootDescriptorTable(2, materialH);
		cmdList->DrawIndexedInstanced(m.indicesNum, 1, idxOffset, 0, 0);
		materialH.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 2;
		idxOffset += m.indicesNum;
	}
}

void PMDClass::Update()
{
	Input* input = new Input();
	input->Initialize();
	input->Update();

	if (input->PushKey(DIK_UP))
	{
		XMFLOAT3 flontVec = c->NormalizeXZ(XMFLOAT3(c->CameraPos().x - position.x, c->CameraPos().y - position.y, c->CameraPos().z - position.z));
		position = XMFLOAT3(position.x - flontVec.x, position.y - flontVec.y, position.z - flontVec.z);
	}
	if (input->PushKey(DIK_DOWN))
	{
		position.z -= 1.0f;
	}
	if (input->PushKey(DIK_LEFT))
	{
		position.x -= 1.0f;

	}
	if (input->PushKey(DIK_RIGHT))
	{
		position.x += 1.0f;
	}
	if (input->PushKey(DIK_SPACE))
	{
	    velocity.y = 1.5f;
	}

	if (input->PushKey(DIK_A))
	{
		c->eyeangleY += 0.03f;
	}
	if (input->PushKey(DIK_D))
	{
		c->eyeangleY -= 0.03f;
	}

	
	rideFlag = false;
	velocity.y += grav.y;
	position = XMFLOAT3(position.x + velocity.x, position.y + velocity.y, position.z + velocity.z);


	//
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


	//

	c->SetEye(position);
	c->SetTarget(position);

	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&Camera::CameraPos()), XMLoadFloat3(&Camera::Target()), XMLoadFloat3(&Camera::Up())
	);

	HRESULT result;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->world = matWorld;
	constMap->viewproj = matView * matProjection;
	constBuff->Unmap(0, nullptr);
	delete input;
	
}

void PMDClass::CollisionFloor(Floor floor)
{
	if (abs(position.x - floor.position.x) < floor.scale.x && abs(position.z - floor.position.z) < floor.scale.z)
	{

		if (position.y + velocity.y <= floor.position.y)
		{
			grav.y = 0;
			velocity.y = 0;
			rideFlag = true;
		}
		else
		{
			grav.y = -0.03f;
		}
	}
	else
	{
		if (!rideFlag)
		{
			grav.y = -0.03;
		}

	}
}

std::string PMDClass::GetTexturePathFtomModelAndTexPath(const std::string & modelpath, const char * texpath)
{
	auto folderaPath = modelpath.substr(0, modelpath.rfind('/')+1);
	return folderaPath + texpath;
}
std::wstring GetWideStringFromString(const std::string& str)
{
	auto num1 = MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.c_str(),
		-1,
		nullptr,
		0
	);
	std::wstring wstr;
	wstr.resize(num1);

	auto num2 = MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.c_str(),
		-1,
		&wstr[0],
		num1
	);
	assert(num1 == num2);
	return wstr;
}
ID3D12Resource* PMDClass::LoadTexTureFromFile(std::string& texpath, ID3D12Device* dev)
{
	TexMetadata metadate = {};
	ScratchImage scratchImg = {};

	HRESULT result;
	result = LoadFromWICFile(
		GetWideStringFromString(texpath).c_str(),
		WIC_FLAGS_NONE,
		&metadate,
		scratchImg
	);
	if (FAILED(result))
	{
		return nullptr;
	}
	const Image* img = scratchImg.GetImage(0, 0, 0);

	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapProp.CreationNodeMask = 0;
	texHeapProp.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resDesc = {};

	resDesc.Format = metadate.format;
	resDesc.Width = metadate.width;
	resDesc.Height = metadate.height;
	resDesc.DepthOrArraySize = metadate.arraySize;
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	resDesc.MipLevels = metadate.mipLevels;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadate.dimension);
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* texBuff = nullptr;

	result = dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);
	if (FAILED(result))
	{
		return nullptr;
	}
	result = texBuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		img->rowPitch,
		img->slicePitch
	);
	if (FAILED(result))
	{
		return nullptr;
	}
	return texBuff;

}
ID3D12Resource* CreateWhiteTexture(ID3D12Device* dev)
{
	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapProp.CreationNodeMask = 0;
	texHeapProp.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resDesc = {};

	resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resDesc.Width = 4;
	resDesc.Height =4;
	resDesc.DepthOrArraySize = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	resDesc.MipLevels = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* texBuff = nullptr;
	HRESULT result;
	result = dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	if (FAILED(result))
	{
		return nullptr;
	}
	std::vector<unsigned char>date(4 * 4 * 4);
	std::fill(date.begin(), date.end(), 0xff);

	result = texBuff->WriteToSubresource(
		0, nullptr,
		date.data(),
		4 * 4,
		date.size()
	);
	return texBuff;
}
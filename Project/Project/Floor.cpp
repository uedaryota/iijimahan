#include "Floor.h"
#include"Camera.h"
#include"DirectXDevice.h"

Floor::Floor()
{
}

Floor::~Floor()
{
	delete(pipelinestate);
	delete(rootsignature);
	delete(mainDescHeap);
	delete(dsvHeap);

}

void Floor::Initialize(ID3D12Device * dev)
{
	CreateMainHeap(dev);
	CreateSubHeap(dev);
	CreatePipeline(dev);
	SetVert(dev);
	SetDepth(dev);
	input = new Input();
	input->Initialize();
}

void Floor::CreatePipeline(ID3D12Device * dev)
{
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT result;
	result = D3DCompileFromFile(
		L"FloorVertexShader.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"FloorPixelShader.hlsl", // シェーダファイル名
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
		},
{
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
	gpipeline.DepthStencilState.StencilEnable = false;

	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;



	D3D12_ROOT_SIGNATURE_DESC rootSigunatureDesc = {};
	rootSigunatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	///////////////////////////////




	D3D12_DESCRIPTOR_RANGE descTblRange[2] = {};
	descTblRange[0].NumDescriptors = 1;
	descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descTblRange[0].BaseShaderRegister = 0;
	descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descTblRange[1].NumDescriptors = 1;
	descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descTblRange[1].BaseShaderRegister = 0;
	descTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	D3D12_ROOT_PARAMETER rootparam[2] = {};

	rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//ALL
	rootparam[0].DescriptorTable.pDescriptorRanges = &descTblRange[0];
	rootparam[0].DescriptorTable.NumDescriptorRanges = 1;

	rootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//ALL
	rootparam[1].DescriptorTable.pDescriptorRanges = &descTblRange[1];
	rootparam[1].DescriptorTable.NumDescriptorRanges = 1;


	rootSigunatureDesc.pParameters = rootparam;
	rootSigunatureDesc.NumParameters = 2;

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
		IID_PPV_ARGS(&rootsignature)
	);
	gpipeline.pRootSignature = rootsignature;
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));



}

void Floor::CreateWallPipeline(ID3D12Device * dev)
{

	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT result;
	result = D3DCompileFromFile(
		L"FloorVertexShader.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"FloorPixelShader.hlsl", // シェーダファイル名
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
		},
{
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
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // カリングしない
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
	gpipeline.DepthStencilState.StencilEnable = false;

	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;



	D3D12_ROOT_SIGNATURE_DESC rootSigunatureDesc = {};
	rootSigunatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	///////////////////////////////







	D3D12_DESCRIPTOR_RANGE descTblRange[2] = {};
	descTblRange[0].NumDescriptors = 1;
	descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descTblRange[0].BaseShaderRegister = 0;
	descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descTblRange[1].NumDescriptors = 1;
	descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descTblRange[1].BaseShaderRegister = 0;
	descTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	

	D3D12_ROOT_PARAMETER rootparam[2] = {};

	rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//ALL
	rootparam[0].DescriptorTable.pDescriptorRanges = &descTblRange[0];
	rootparam[0].DescriptorTable.NumDescriptorRanges = 1;

	rootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//ALL
	rootparam[1].DescriptorTable.pDescriptorRanges = &descTblRange[1];
	rootparam[1].DescriptorTable.NumDescriptorRanges = 1;

	
	rootSigunatureDesc.pParameters = rootparam;
	rootSigunatureDesc.NumParameters = 2;
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
		IID_PPV_ARGS(&rootsignature)
	);
	gpipeline.pRootSignature = rootsignature;
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));



}

void Floor::CreateMainHeap(ID3D12Device * dev)
{

	HRESULT result;

	TexMetadata metadate = {};
	ScratchImage scratchImg = {};
	result = LoadFromWICFile(
		L"img/床.png",
		WIC_FLAGS_NONE,
		&metadate,
		scratchImg
	);

	const Image* img = scratchImg.GetImage(0, 0, 0);


	D3D12_HEAP_PROPERTIES texheapprop = {};
	texheapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	texheapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texheapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texheapprop.CreationNodeMask = 0;
	texheapprop.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC texresdes = {};

	texresdes.Format = metadate.format;
	texresdes.Width = img->width;
	texresdes.Height = img->height;
	texresdes.DepthOrArraySize = metadate.arraySize;
	texresdes.SampleDesc.Count = 1;
	texresdes.SampleDesc.Quality = 0;
	texresdes.MipLevels = metadate.mipLevels;
	texresdes.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadate.dimension);
	texresdes.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texresdes.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* texBuff = nullptr;



	result = dev->CreateCommittedResource(
		&texheapprop,
		D3D12_HEAP_FLAG_NONE,
		&texresdes,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	result = texBuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		img->rowPitch,
		img->slicePitch
	);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = metadate.format;//resdescと合わせる
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;




	mainDescHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};

	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 2;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&mainDescHeap));


	D3D12_CPU_DESCRIPTOR_HANDLE HeapHandle = mainDescHeap->GetCPUDescriptorHandleForHeapStart();

	dev->CreateShaderResourceView(
		texBuff,
		&srvDesc,
		HeapHandle);

	HeapHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);





	//HeapHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

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
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;



	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatrocesDate) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);


	/*result = constBuff->Map(0, nullptr, (void**)&constMap);

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
*/

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = constBuff->GetDesc().Width;
	dev->CreateConstantBufferView(&cbvDesc, HeapHandle);
	constBuff->Unmap(0, nullptr);

	HeapHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);



}

void Floor::CreateSubHeap(ID3D12Device * dev)
{
	HRESULT result;

	TexMetadata metadate = {};
	ScratchImage scratchImg = {};
	result = LoadFromWICFile(
		L"img/床.png",
		WIC_FLAGS_NONE,
		&metadate,
		scratchImg
	);

	const Image* img = scratchImg.GetImage(0, 0, 0);


	D3D12_HEAP_PROPERTIES texheapprop = {};
	texheapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	texheapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texheapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texheapprop.CreationNodeMask = 0;
	texheapprop.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC texresdes = {};

	texresdes.Format = metadate.format;
	texresdes.Width = img->width;
	texresdes.Height = img->height;
	texresdes.DepthOrArraySize = metadate.arraySize;
	texresdes.SampleDesc.Count = 1;
	texresdes.SampleDesc.Quality = 0;
	texresdes.MipLevels = metadate.mipLevels;
	texresdes.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadate.dimension);
	texresdes.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texresdes.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* texBuff = nullptr;



	result = dev->CreateCommittedResource(
		&texheapprop,
		D3D12_HEAP_FLAG_NONE,
		&texresdes,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	result = texBuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		img->rowPitch,
		img->slicePitch
	);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = metadate.format;//resdescと合わせる
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;




	subDescHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};

	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 2;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&subDescHeap));


	D3D12_CPU_DESCRIPTOR_HANDLE HeapHandle = subDescHeap->GetCPUDescriptorHandleForHeapStart();

	dev->CreateShaderResourceView(
		texBuff,
		&srvDesc,
		HeapHandle);

	HeapHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);





	//HeapHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

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
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;



	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatrocesDate) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff2)
	);



	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constBuff2->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = constBuff2->GetDesc().Width;
	dev->CreateConstantBufferView(&cbvDesc, HeapHandle);
	constBuff2->Unmap(0, nullptr);

	HeapHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	
}

void Floor::ResetTex(const wchar_t* Texname,ID3D12Device * dev)
{

	HRESULT result;

	TexMetadata metadate = {};
	ScratchImage scratchImg = {};
	result = LoadFromWICFile(
		Texname,
		WIC_FLAGS_NONE,
		&metadate,
		scratchImg
	);

	const Image* img = scratchImg.GetImage(0, 0, 0);


	D3D12_HEAP_PROPERTIES texheapprop = {};
	texheapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	texheapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texheapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texheapprop.CreationNodeMask = 0;
	texheapprop.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC texresdes = {};

	texresdes.Format = metadate.format;
	texresdes.Width = img->width;
	texresdes.Height = img->height;
	texresdes.DepthOrArraySize = metadate.arraySize;
	texresdes.SampleDesc.Count = 1;
	texresdes.SampleDesc.Quality = 0;
	texresdes.MipLevels = metadate.mipLevels;
	texresdes.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadate.dimension);
	texresdes.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texresdes.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* texBuff = nullptr;



	result = dev->CreateCommittedResource(
		&texheapprop,
		D3D12_HEAP_FLAG_NONE,
		&texresdes,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	result = texBuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		img->rowPitch,
		img->slicePitch
	);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = metadate.format;//resdescと合わせる
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;






	D3D12_CPU_DESCRIPTOR_HANDLE HeapHandle = mainDescHeap->GetCPUDescriptorHandleForHeapStart();

	dev->CreateShaderResourceView(
		texBuff,
		&srvDesc,
		HeapHandle);

	 HeapHandle = subDescHeap->GetCPUDescriptorHandleForHeapStart();

	dev->CreateShaderResourceView(
		texBuff,
		&srvDesc,
		HeapHandle);



}

void Floor::SetDepth(ID3D12Device * dev)
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

	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	dev->CreateDepthStencilView(
		depthBuff,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

}

void Floor::SetVert(ID3D12Device * dev)
{
	D3D12_HEAP_PROPERTIES vertheapprop = {};
	vertheapprop.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU への転送用
	vertheapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	vertheapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC vertresdesc = {};
	vertresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertresdesc.Width = sizeof(vertices); // 頂点情報が入る分のサイズ
	vertresdesc.Height = 1;
	vertresdesc.DepthOrArraySize = 1;
	vertresdesc.Format = DXGI_FORMAT_UNKNOWN;
	vertresdesc.MipLevels = 1;
	vertresdesc.SampleDesc.Count = 1;
	vertresdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	vertresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ID3D12Resource* vertBuff;
	vertBuff = nullptr;
	HRESULT result;
	result = dev->CreateCommittedResource(
		&vertheapprop, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&vertresdesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	vertBuff->Unmap(0, nullptr);
	vbView = {};
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);



	ID3D12Resource* indexBuff = nullptr;
	vertresdesc.Width = sizeof(indices); // インデックス情報が入る分のサイズ
	  // GPU リソースの生成 	
	result = dev->CreateCommittedResource(&vertheapprop, // ヒープ設定
		D3D12_HEAP_FLAG_NONE, &vertresdesc, // リソース設定 
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	//
	// GPU 上のバッファに対応した仮想メモリを取得
	unsigned short* indexMap = nullptr;

	result = indexBuff->Map(0, nullptr, (void**)&indexMap); // 全インデックスに対して
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i]; // インデックスをコピー

	}
	indexBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);

}

void Floor::Draw(ID3D12GraphicsCommandList * cmdList, ID3D12Device * dev)
{
	if (Camera::ReturnCurrentCamera() == CurrentCamera::Main)
	{
		DirectXDevice::cmdList->RSSetViewports(1, &DirectXDevice::viewport);

		cmdList->SetPipelineState(pipelinestate);
		cmdList->SetGraphicsRootSignature(rootsignature);
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmdList->IASetVertexBuffers(0, 1, &vbView);
		cmdList->IASetIndexBuffer(&ibView);
		cmdList->SetDescriptorHeaps(1, &mainDescHeap);
		D3D12_GPU_DESCRIPTOR_HANDLE handle = mainDescHeap->GetGPUDescriptorHandleForHeapStart();
		cmdList->SetGraphicsRootDescriptorTable(0, handle);

		handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		cmdList->SetGraphicsRootDescriptorTable(1, handle);

		cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
		//


		DirectXDevice::cmdList->RSSetViewports(1, &DirectXDevice::viewport2);


		cmdList->SetDescriptorHeaps(1, &subDescHeap);
		handle = subDescHeap->GetGPUDescriptorHandleForHeapStart();
		cmdList->SetGraphicsRootDescriptorTable(0, handle);

		handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		cmdList->SetGraphicsRootDescriptorTable(1, handle);

		cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
		//


	}
	else if (Camera::ReturnCurrentCamera() == CurrentCamera::Sub)
	{
		DirectXDevice::cmdList->RSSetViewports(1, &DirectXDevice::viewport);
		cmdList->SetPipelineState(pipelinestate);
		cmdList->SetGraphicsRootSignature(rootsignature);
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmdList->IASetVertexBuffers(0, 1, &vbView);
		cmdList->IASetIndexBuffer(&ibView);
		D3D12_GPU_DESCRIPTOR_HANDLE handle = subDescHeap->GetGPUDescriptorHandleForHeapStart();
		//
		cmdList->SetDescriptorHeaps(1, &subDescHeap);
		
		cmdList->SetGraphicsRootDescriptorTable(0, handle);

		handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		cmdList->SetGraphicsRootDescriptorTable(1, handle);

		cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);

	
		//


		DirectXDevice::cmdList->RSSetViewports(1, &DirectXDevice::viewport2);

		//
		handle = mainDescHeap->GetGPUDescriptorHandleForHeapStart();
		cmdList->SetDescriptorHeaps(1, &mainDescHeap);
		
		cmdList->SetGraphicsRootDescriptorTable(0, handle);

		handle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		cmdList->SetGraphicsRootDescriptorTable(1, handle);

		cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
		//

	}
}

void Floor::Update()
{
	HRESULT result;
	
	input->Update();
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
	

	result = constBuff->Map(0, nullptr, (void**)&constMap);

	constMap->world = matWorld;
	constMap->viewproj = Camera::ReturnCameraState()->matView *  Camera::ReturnCameraState()->matProjection;
	constMap->alpha = constalpha;
	constBuff->Unmap(0, nullptr);

	result = constBuff2->Map(0, nullptr, (void**)&constMap2);

	constMap2->world = matWorld;
	constMap2->viewproj = Camera::ReturnSubCameraState()->matView *  Camera::ReturnSubCameraState()->matProjection;
	constMap2->alpha = constalpha;

	constBuff2->Unmap(0, nullptr);



	//
	if (input->PushKey(DIK_B))
	{
		Camera::SetTarget(XMFLOAT3(10, 0, 0));
	}
	//c.SetEye(position);

	



}

void Floor::SetPos(XMFLOAT3 pos)
{
	position = pos;
}

void Floor::SetScale(XMFLOAT3 Scale)
{
	scale = Scale;
}

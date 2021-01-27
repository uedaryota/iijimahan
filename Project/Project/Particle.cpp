#include "Particle.h"
#include"DirectXDevice.h"
#include"Camera.h"
ID3D12PipelineState* Particle::pipelinestate = nullptr;
ID3D12RootSignature* Particle::rootsignature = nullptr;
//ID3D12DescriptorHeap* Particle::mainDescHeap = nullptr;
D3D12_VERTEX_BUFFER_VIEW Particle::vbView = {};
D3D12_INDEX_BUFFER_VIEW Particle::ibView;
XMMATRIX Particle::matView;


//static const int size = 0.5f;
Particle::Vertex Particle::vertices[4] =
{
	{{-0.5f,-0.5f,0},{0.0f,1.0f}},
{{-0.5f,0.5f,0},{0.0f,0.0f} },
{{0.5f,-0.5f,0} , { 1.0f,1.0f }},
{{0.5f,0.5f,0} , { 1.0f,0.0f }},

};

unsigned short Particle::indices[6] =
{
0,1,2,
2,1,3
};
ID3D12Resource* Particle::vertBuff = nullptr;
ID3D12Resource* Particle::indexBuff = nullptr;
ID3D12Resource* Particle::texBuff = nullptr;

Particle::Vertex* Particle::vertMap = nullptr;
unsigned short* Particle::indexMap = nullptr;

Particle::Particle()
{
}

Particle::~Particle()
{
}

void Particle::Initialize()
{
	CreateMainHeap();
	CreatePipeline();
	SetVert();

}

void Particle::CreatePipeline()
{
	if (pipelinestate == nullptr&&rootsignature == nullptr)
	{
		ID3DBlob* vsBlob = nullptr;
		ID3DBlob* psBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		HRESULT result;
		result = D3DCompileFromFile(
			L"ParticleVertexShader.hlsl", // シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&vsBlob, &errorBlob);
		// ピクセルシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			L"ParticlePixelShader.hlsl", // シェーダファイル名
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

		gpipeline.DepthStencilState.DepthEnable = false;//被っても描画
		gpipeline.DepthStencilState.StencilEnable = false;
		gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
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

		result = DirectXDevice::dev->CreateRootSignature(
			0,
			rootSigBlob->GetBufferPointer(),
			rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(&rootsignature)
		);
		gpipeline.pRootSignature = rootsignature;
		result = DirectXDevice::dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	}
}

void Particle::CreateMainHeap()
{
	
		HRESULT result;
		//if (texBuff == nullptr);
		//{

		//}
		TexMetadata metadate = {};
		ScratchImage scratchImg = {};
		result = LoadFromWICFile(
			L"img/Particle3.png",
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


		texBuff = nullptr;



		result = DirectXDevice::dev->CreateCommittedResource(
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

		result = DirectXDevice::dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&mainDescHeap));


		D3D12_CPU_DESCRIPTOR_HANDLE HeapHandle = mainDescHeap->GetCPUDescriptorHandleForHeapStart();

		DirectXDevice::dev->CreateShaderResourceView(
			texBuff,
			&srvDesc,
			HeapHandle);

		HeapHandle.ptr += DirectXDevice::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&Camera::MainCameraPos()), XMLoadFloat3(&Camera::Target()), XMLoadFloat3(&Camera::Up())
	);

	result = DirectXDevice::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatrocesDate) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = constBuff->GetDesc().Width;
	DirectXDevice::dev->CreateConstantBufferView(&cbvDesc, HeapHandle);
	result = constBuff->Map(0, nullptr, (void**)&constMap);

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

	color = { 1,1,1,1 };
	constMap->world = matWorld;
	constMap->viewproj = matView * Camera::ReturnCameraState()->matProjection;
	constMap->color = color;

	constBuff->Unmap(0, nullptr);
}

void Particle::SetTex(const wchar_t* texname)
{
	HRESULT result;

	TexMetadata metadate = {};
	ScratchImage scratchImg = {};
	result = LoadFromWICFile(
		texname,
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

	texBuff = nullptr;



	result = DirectXDevice::dev->CreateCommittedResource(
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

	DirectXDevice::dev->CreateShaderResourceView(
		texBuff,
		&srvDesc,
		HeapHandle);
}


void Particle::SetVert()
{
	if (vertMap == nullptr&&indexMap == nullptr)
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


		vertBuff = nullptr;
		HRESULT result;
		result = DirectXDevice::dev->CreateCommittedResource(
			&vertheapprop, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&vertresdesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff));

		vertMap = nullptr;
		result = vertBuff->Map(0, nullptr, (void**)&vertMap);
		std::copy(std::begin(vertices), std::end(vertices), vertMap);
		vertBuff->Unmap(0, nullptr);
		vbView = {};
		vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
		vbView.SizeInBytes = sizeof(vertices);
		vbView.StrideInBytes = sizeof(vertices[0]);

		indexBuff = nullptr;
		vertresdesc.Width = sizeof(indices); // インデックス情報が入る分のサイズ
		  // GPU リソースの生成 	
		result = DirectXDevice::dev->CreateCommittedResource(&vertheapprop, // ヒープ設定
			D3D12_HEAP_FLAG_NONE, &vertresdesc, // リソース設定 
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff));
		//
		// GPU 上のバッファに対応した仮想メモリを取得
		indexMap = nullptr;

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
}

void Particle::Draw()
{
	DirectXDevice::cmdList->SetPipelineState(pipelinestate);
	DirectXDevice::cmdList->SetGraphicsRootSignature(rootsignature);
	DirectXDevice::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectXDevice::cmdList->IASetVertexBuffers(0, 1, &vbView);
	DirectXDevice::cmdList->IASetIndexBuffer(&ibView);
	DirectXDevice::cmdList->SetDescriptorHeaps(1, &mainDescHeap);
	D3D12_GPU_DESCRIPTOR_HANDLE handle = mainDescHeap->GetGPUDescriptorHandleForHeapStart();
	DirectXDevice::cmdList->SetGraphicsRootDescriptorTable(0, handle);

	handle.ptr += DirectXDevice::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DirectXDevice::cmdList->SetGraphicsRootDescriptorTable(1, handle);
	DirectXDevice::cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);

}

void Particle::Update()
{

	//
	XMMATRIX bill = SetBillboard();
	
	speed += addspeed;
	addspeed = addspeed - addspeed / 10;
	if (speed < 0.2f*(scale.x / 350))
	{
		speed = 0.2f*(scale.x / 350);
	}
	position = { position.x + velocity.x*speed, position.y + velocity.y*speed, position.z + velocity.z*speed };
	life--;
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
	matWorld *= bill;

	//color.x -= 0.01f;

	color.y -= 1 / life * 3; 
	color.z -= 1 / life * 3;
	color.x -= 1 / life / 2;
	if (color.y < minColor && color.z < minColor)
	{
		color.y = minColor;
		color.z = minColor;
	
		if (color.x < minColor)
		{
			color.x -= minColor;
		}
		color.w -= 1 / life/2;
	}
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	
	constMap->world = matWorld;
	constMap->viewproj = matView * Camera::ReturnCameraState()->matProjection;
	constMap->color = color;
	constBuff->Unmap(0, nullptr);
}

void Particle::SetPos(XMFLOAT3 pos)
{
	position = pos;
}

void Particle::SetScale(XMFLOAT3 Scale)
{
	scale = Scale;
}

XMMATRIX Particle::SetBillboard()
{
	XMMATRIX matBillboard;

	XMVECTOR eyePos = XMLoadFloat3(&Camera::MainCameraPos());
	XMVECTOR targetPos = XMLoadFloat3(&Camera::Target());
	XMVECTOR upVector = XMLoadFloat3(&Camera::ReturnCameraState()->up);

	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPos, eyePos);
	//
	//０ベクトルだと除外
	//
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	XMVECTOR cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);


	XMVECTOR cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	XMMATRIX matcameraRot;
	matcameraRot.r[0] = cameraAxisX;
	matcameraRot.r[1] = cameraAxisY;
	matcameraRot.r[2] = cameraAxisZ;
	matcameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	matView = XMMatrixTranspose(matcameraRot);

	XMVECTOR reverseEyePos = XMVectorNegate(eyePos);
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePos);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePos);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePos);

	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	matView.r[3] = translation;

	matBillboard = XMMatrixIdentity();
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
	return matBillboard;
}

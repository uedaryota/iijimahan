#include "CircleCollision.h"
#include"Camera.h"
#include"DirectXDevice.h"
CircleCollision::CircleCollision()
{
	Initialize();
}

CircleCollision::~CircleCollision()
{
}

void CircleCollision::Update()
{
	HRESULT result;



	XMVECTOR eyePos = XMLoadFloat3(&Camera::MainCameraPos());
	XMVECTOR targetPos = XMLoadFloat3(&Camera::Target());
	XMVECTOR upVector = XMLoadFloat3(&Camera::ReturnCameraState()->up);

	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPos, eyePos);
	//
	//�O�x�N�g�����Ə��O
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

	result = constBuff->Map(0, nullptr, (void**)&constMap);

	matScale = XMMatrixScaling(scale, scale, scale);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matRot *= XMMatrixRotationZ(rotation.z);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	matWorld = XMMatrixIdentity();


	matWorld *= matBillboard;
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	constMap->world = matWorld;
	constMap->viewproj = matView * matProjection;
	constMap->color = color;
	constBuff->Unmap(0, nullptr);


}

bool CircleCollision::CollisionUpdate(CircleCollision other)
{
	HRESULT result;


	XMVECTOR eyePos = XMLoadFloat3(&Camera::MainCameraPos());
	XMVECTOR targetPos = XMLoadFloat3(&Camera::Target());
	XMVECTOR upVector = XMLoadFloat3(&Camera::ReturnCameraState()->up);

	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPos, eyePos);
	//
	//�O�x�N�g�����Ə��O
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

	XMMATRIX matBillboard = XMMatrixIdentity();
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);


	result = constBuff->Map(0, nullptr, (void**)&constMap);

	matScale = XMMatrixScaling(scale, scale, scale);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matRot *= XMMatrixRotationZ(rotation.z);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	matWorld = XMMatrixIdentity();
	matWorld *= matBillboard;
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	constMap->world = matWorld;
	constMap->viewproj = matView * matProjection;
	constMap->color = color;
	constBuff->Unmap(0, nullptr);


	return false;
}

void CircleCollision::SetMainHeap()
{
	HRESULT result;
	mainDescHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};

	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 1;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = DirectXDevice::dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&mainDescHeap));


	D3D12_CPU_DESCRIPTOR_HANDLE HeapHandle = mainDescHeap->GetCPUDescriptorHandleForHeapStart();


	result = DirectXDevice::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstDate) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);



	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = constBuff->GetDesc().Width;
	DirectXDevice::dev->CreateConstantBufferView(&cbvDesc, HeapHandle);
}

void CircleCollision::SetPipeLine()
{

	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT result;
	result = D3DCompileFromFile(
		L"ColliderVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"ColliderPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
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



	};








	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};


	gpipeline.pRootSignature = nullptr;
	gpipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = psBlob->GetBufferSize();

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	gpipeline.RasterizerState.MultisampleEnable = false;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	gpipeline.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	gpipeline.BlendState.AlphaToCoverageEnable = false;
	gpipeline.BlendState.IndependentBlendEnable = false;
	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�(8 ���邪���܂͈�����g��Ȃ�) 
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // �W���ݒ�
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;  // ���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;  // �\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // 1.0f-�\�[�X�̃A���t�@�l 

	// �u�����h�X�e�[�g�ɐݒ肷��
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // �`��Ώۂ� 1 ��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0~255 �w��� RGBA
	gpipeline.SampleDesc.Count = 1; // 1 �s�N�Z���ɂ� 1 ��T���v�����O
	gpipeline.SampleDesc.Quality = 0;

	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.DepthStencilState.StencilEnable = false;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;



	D3D12_ROOT_SIGNATURE_DESC rootSigunatureDesc = {};
	rootSigunatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	///////////////////////////////




	D3D12_DESCRIPTOR_RANGE descTblRange[1] = {};
	descTblRange[0].NumDescriptors = 1;
	descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descTblRange[0].BaseShaderRegister = 0;
	descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;



	D3D12_ROOT_PARAMETER rootparam[1] = {};

	rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//ALL
	rootparam[0].DescriptorTable.pDescriptorRanges = &descTblRange[0];
	rootparam[0].DescriptorTable.NumDescriptorRanges = 1;


	rootSigunatureDesc.pParameters = rootparam;
	rootSigunatureDesc.NumParameters = 1;

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

void CircleCollision::SetMatrix()
{
	HRESULT result;



	matProjection = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast<float>(Camera::window_width) / static_cast<float>(Camera::window_height),
		1.0f,
		1000.0f
	);
	//

	XMVECTOR eyePos = XMLoadFloat3(&Camera::MainCameraPos());
	XMVECTOR targetPos = XMLoadFloat3(&Camera::Target());
	XMVECTOR upVector = XMLoadFloat3(&Camera::ReturnCameraState()->up);

	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPos, eyePos);
	//
	//�O�x�N�g�����Ə��O
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

	//
	result = constBuff->Map(0, nullptr, (void**)&constMap);

	matScale = XMMatrixScaling(scale, scale, scale);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matRot *= XMMatrixRotationZ(rotation.z);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	constMap->world = matWorld;
	constMap->viewproj = matView * matProjection;
	constMap->color = color;
	constBuff->Unmap(0, nullptr);
}

void CircleCollision::SetVert()
{
	for (int a = 0; a < vertnum; a++)
	{
		vertices[a].pos.x = float(1.0f * sin(2 * 3.14f / vertnum * a));

		vertices[a].pos.y = float(1.0f * cos(2 * 3.14f / vertnum * a));

		vertices[a].pos.z = 0;

	}
	vertices[vertnum].pos.x = 0;
	vertices[vertnum].pos.y = 0;
	vertices[vertnum].pos.z = 0;




	for (int a = 0; a < vertnum * 3; a += 3)
	{
		indices[a] = vertnum;
		indices[a + 1] = a / 3 + 1;
		indices[a + 2] = a / 3;//���\���킹�邽�ߓ���ւ�

	}
	indices[vertnum * 3 - 2] = 0;
	//for (int a = 0; a < _countof(indices) / 2; a += 6)
	//{
	//	indices[a + _countof(indices) / 2] = a / 6;
	//	indices[a + 1 + _countof(indices) / 2] = a / 6 + 1;
	//	indices[a + 2 + _countof(indices) / 2] = a / 6 + vertnum + 1;//���\���킹�邽�ߓ���ւ�

	//	indices[a + 3 + _countof(indices) / 2] = a / 6 + 1;
	//	indices[a + 4 + _countof(indices) / 2] = a / 6 + vertnum + 1;
	//	indices[a + 5 + _countof(indices) / 2] = a / 6 + vertnum + 2;
	//}
	//indices[vertnum * 3 - 2] = 1;
	//indices[vertnum * 3 * 2 - 1] = vertnum + 1;
	//indices[vertnum * 3 * 2 * 2 - 3] = 0;
	//indices[vertnum * 3 * 2 * 2 - 5] = 0;


	D3D12_HEAP_PROPERTIES vertheapprop = {};
	vertheapprop.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU �ւ̓]���p
	vertheapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	vertheapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC vertresdesc = {};
	vertresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertresdesc.Width = sizeof(vertices); // ���_��񂪓��镪�̃T�C�Y
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
		&vertheapprop, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&vertresdesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));


	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	vertBuff->Unmap(0, nullptr);
	vbView = {};
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);



	vertresdesc.Width = sizeof(indices); // �C���f�b�N�X��񂪓��镪�̃T�C�Y
	  // GPU ���\�[�X�̐��� 	
	result = DirectXDevice::dev->CreateCommittedResource(&vertheapprop, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE, &vertresdesc, // ���\�[�X�ݒ� 
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	//
	// GPU ��̃o�b�t�@�ɑΉ��������z���������擾

	result = indexBuff->Map(0, nullptr, (void**)&indexMap); // �S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i]; // �C���f�b�N�X���R�s�[

	}
	indexBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);

}

void CircleCollision::Draw()
{
	DirectXDevice::cmdList->SetPipelineState(pipelinestate);
	DirectXDevice::cmdList->SetGraphicsRootSignature(rootsignature);
	DirectXDevice::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectXDevice::cmdList->IASetVertexBuffers(0, 1, &vbView);
	DirectXDevice::cmdList->IASetIndexBuffer(&ibView);
	DirectXDevice::cmdList->SetDescriptorHeaps(1, &mainDescHeap);

	D3D12_GPU_DESCRIPTOR_HANDLE handle = mainDescHeap->GetGPUDescriptorHandleForHeapStart();
	DirectXDevice::cmdList->SetGraphicsRootDescriptorTable(0, handle);
	DirectXDevice::cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void CircleCollision::Initialize()
{
	SetMainHeap();
	SetPipeLine();
	SetMatrix();
	SetVert();
}

void CircleCollision::SetPos(XMFLOAT3 pos)
{
	position = pos;
}

void CircleCollision::AddPos(XMFLOAT3 pos)
{
	position.x += pos.x;
	position.y += pos.y;
	position.z += pos.z;

}

void CircleCollision::SetLength(float len)
{
}


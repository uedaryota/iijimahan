
#include "DirectXDevice.h"
#include <comutil.h>
#include "Input.h"
#include"Texture.h"
#include"Stage2D.h"
ID3D12GraphicsCommandList* DirectXDevice::cmdList = nullptr;;
ID3D12Device* DirectXDevice::dev = nullptr;
IDXGIFactory6*  DirectXDevice::dxgifactory;
IDXGISwapChain4*  DirectXDevice::swapchain{};
D3D12_VIEWPORT DirectXDevice::viewport;
D3D12_VIEWPORT DirectXDevice::viewport2;
D3D12_RECT  DirectXDevice::scissorrect;
ID3D12CommandAllocator*  DirectXDevice::cmdAllocator;
ID3D12CommandQueue* DirectXDevice::cmdQueue = nullptr;
HWND DirectXDevice::hwnd;
ID3D12DescriptorHeap* DirectXDevice::rtvHeaps = nullptr;
float DirectXDevice::clearColor[] = { 1.0f,1.0f,1.0f,0.0f };
D3D12_CPU_DESCRIPTOR_HANDLE DirectXDevice::rtvH;
std::vector<ID3D12Resource*>DirectXDevice::backBuffers;
ID3D12DescriptorHeap* DirectXDevice::dsvHeap = nullptr;
ID3D12Resource* DirectXDevice::depthBuff = nullptr;
ID3D12Fence* DirectXDevice::fence = nullptr;
UINT64 DirectXDevice::fenceVal = 0;
WNDCLASSEX DirectXDevice::w{};

Camera* c = new Camera();
PMDClass* pmd = new PMDClass();
Floor* floor1 = new Floor();
Tower* tower = new Tower();
XMFLOAT3 pointA = { 15,15,15 };
XMFLOAT3 pointB = { -15,-15,-15 };
Block* block = new Block();
Sound* sound = new Sound();
//Stage* stage = new Stage();
Stage2D* stage = new Stage2D();
Enemy* enemy = new Enemy();
Enemy* enemy2 = new Enemy();
EnemyAI* ai = new EnemyAI();
EnemyManeger* manager = new EnemyManeger();
Sprite* back = new Sprite();
Input* input = new Input();
Texture* tex = new Texture();
LRESULT WindowProc1(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void DirectXDevice::Initialize()
{
	CreateGameWindow();
	CreateDevice();
	CreatecmdList_Queue();
	CreateSwapchain();
	CreateRTV();
	CreateDsv();
	SetView_Scissor();
	SetFence();
	block->Initialize();
	tower->Initialize(DirectXDevice::dev);
	floor1->Initialize(DirectXDevice::dev);
	manager->Initialize();
	sound->Initialize();
	stage->Initialize();
	manager->Add(enemy);
	//manager->Add(enemy2);
	enemy->state = move1;

	tex->Initialize();
	back->Initialize();
	back->ResetTex(L"img/Blueback.png");
	back->SetScale(XMFLOAT3(300, 300, 300));
	back->SetPos(XMFLOAT3(0, 0, 500));
	enemy->SetTower(tower);
	//enemy->SetTarget(&tower->GetPosition);
	//enemy2->state = move3;
	//enemy2->SetPos(XMFLOAT3{ 0.0f,240.0f,0.0f });
	sound->LoadFile(L".\\Resources\\01.mp3");
	input->Initialize();
}
	
void DirectXDevice::Update()
{
	HRESULT result;
	result = DirectXDevice::cmdAllocator->Reset();
	UINT bbIdx = DirectXDevice::swapchain->GetCurrentBackBufferIndex();
	rtvH = DirectXDevice::rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIdx * DirectXDevice::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);


	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrierDesc.Transition.Subresource = 0;

	barrierDesc.Transition.pResource = DirectXDevice::backBuffers[bbIdx];
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	cmdList->ResourceBarrier(1, &barrierDesc);



	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = DirectXDevice::dsvHeap->GetCPUDescriptorHandleForHeapStart();

	DirectXDevice::cmdList->OMSetRenderTargets(1, &rtvH, true, &dsvH);
	DirectXDevice::cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	
	DirectXDevice::cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	DirectXDevice::cmdList->ResourceBarrier(1, &barrierDesc);

	viewport2.Width = Camera::window_width/3;
	viewport2.Height = Camera::window_height/3;
	viewport2.TopLeftX = Camera::window_width / 3 * 2;
	viewport2.TopLeftY = 0;
	viewport2.MaxDepth = 0.5f;
	viewport2.MinDepth = 0.0f;

	DirectXDevice::cmdList->RSSetViewports(1, &viewport);

	DirectXDevice::cmdList->RSSetScissorRects(1, &scissorrect);
	//ここからUpdate

	Camera::Update();
	tower->Update();
//	sound->Update();
	
	tower->Draw(DirectXDevice::cmdList);

	manager->Update();
	manager->Draw();
	/*back->Update();
	back->Draw();
	*/
	stage->Update();
	stage->Draw();


	if (input->PushKey(DIK_Q))//実験用→実験結果成功　＊座標の変更を行えます。
	{
		manager->AncerSet(XMFLOAT3{ 15,15,15 }, XMFLOAT3{ 15,15,15 });
	}
	if (input->PushKey(DIK_A))//実験用→実験結果成功　＊座標の変更を行えます。
	{
		manager->ReAncerSet(XMFLOAT3{ -100,1,-100 }, XMFLOAT3{ 500,500,500 });
	}
	if (input->PushKey(DIK_Z))
	{
		enemy->EnemyDamege(2);
	}
	//DirectXDevice::cmdList->RSSetViewports(1, &viewport2);
	//
	//tower->Draw(DirectXDevice::cmdList);

//	stage->Update();
	//stage->Draw();
	//manager->Draw();

	//ここまで
	DirectXDevice::cmdList->Close();
	input->Update();

	if (input->PushKey(DIK_P))
	{
		sound->PlayRoop();
	}
	

	ID3D12CommandList* cmdLists[] = { DirectXDevice::cmdList };
	cmdQueue->ExecuteCommandLists(1, cmdLists);


	cmdQueue->Signal(DirectXDevice::fence, ++DirectXDevice::fenceVal);
	while (fence->GetCompletedValue() != DirectXDevice::fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(DirectXDevice::fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}


	DirectXDevice::cmdAllocator->Reset();
	DirectXDevice::cmdList->Reset(cmdAllocator, nullptr);

	DirectXDevice::swapchain->Present(1, 0);
}
void DirectXDevice::CreateGameWindow()
{


	WNDCLASSEX w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc1;
	w.lpszClassName = (L"DXSample");
	w.hInstance = GetModuleHandle(nullptr);
	RegisterClassEx(&w);

	RECT wrc = { 0,0,Camera::window_width,Camera::window_height };

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	DirectXDevice::hwnd = CreateWindow(w.lpszClassName,
		L"DXTest",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w.hInstance,
		nullptr);




	ShowWindow(hwnd, SW_SHOW);

}
void DirectXDevice::CreateDevice()
{

	D3D_FEATURE_LEVEL levels[] =
	{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;


	for (int i = 0; i < _countof(levels); i++)
	{

		if (SUCCEEDED(D3D12CreateDevice(nullptr, levels[i], IID_PPV_ARGS(&DirectXDevice::dev))))
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
	HRESULT result;
#ifdef DEBUG
	CreateDXGIFactory1(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&dxgifactory));
#else
	result = CreateDXGIFactory1(IID_PPV_ARGS(&DirectXDevice::dxgifactory));
#endif 

	std::vector<IDXGIAdapter*>adapters;

	IDXGIAdapter* tmpAdapter = nullptr;
	for (int i = 0;
		DirectXDevice::dxgifactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);
	}
	for (auto adpt : adapters)
	{
		DXGI_ADAPTER_DESC adesc = {};
		adpt->GetDesc(&adesc);
		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"NVIDIA") != std::string::npos)
		{
			tmpAdapter = adpt;
			break;
		}
	}
}

void DirectXDevice::CreatecmdList_Queue()
{
	HRESULT result;
	result = DirectXDevice::dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&DirectXDevice::cmdAllocator));
	result = DirectXDevice::dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		DirectXDevice::cmdAllocator, nullptr,
		IID_PPV_ARGS(&DirectXDevice::cmdList));

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc;

	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	result = DirectXDevice::dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&DirectXDevice::cmdQueue));

}
void DirectXDevice::CreateSwapchain()
{

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	HRESULT result;

	swapchainDesc.Width = Camera::window_width;
	swapchainDesc.Height = Camera::window_height;
	swapchainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapchainDesc.Stereo = false;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;

	swapchainDesc.Scaling = DXGI_SCALING_STRETCH;

	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	result = DirectXDevice::dxgifactory->CreateSwapChainForHwnd(
		DirectXDevice::cmdQueue,
		DirectXDevice::hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&DirectXDevice::swapchain
	);
}
void DirectXDevice::CreateRTV()
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 2;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT result = DirectXDevice::dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&DirectXDevice::rtvHeaps));

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = DirectXDevice::swapchain->GetDesc(&swcDesc);

	DirectXDevice::backBuffers.resize(swcDesc.BufferCount);

	D3D12_CPU_DESCRIPTOR_HANDLE handle =
		DirectXDevice::rtvHeaps->GetCPUDescriptorHandleForHeapStart();

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for (int idx = 0; idx < swcDesc.BufferCount; ++idx)
	{
		result = DirectXDevice::swapchain->GetBuffer(idx, IID_PPV_ARGS(&DirectXDevice::backBuffers[idx]));


		//dev->CreateRenderTargetView(backBuffers[idx], &rtvDesc, handle);


		DirectXDevice::dev->CreateRenderTargetView(DirectXDevice::backBuffers[idx], nullptr, handle);

		handle.ptr += DirectXDevice::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	}

}
void DirectXDevice::CreateDsv()
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


	HRESULT result;
	result = DirectXDevice::dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthresdesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&DirectXDevice::depthBuff)
	);

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	result = DirectXDevice::dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&DirectXDevice::dsvHeap));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	DirectXDevice::dev->CreateDepthStencilView(
		DirectXDevice::depthBuff,
		&dsvDesc,
		DirectXDevice::dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void DirectXDevice::SetView_Scissor()
{

	DirectXDevice::viewport.Width = Camera::window_width;
	DirectXDevice::viewport.Height = Camera::window_height;
	DirectXDevice::viewport.TopLeftX = 0;
	DirectXDevice::viewport.TopLeftY = 0;
	DirectXDevice::viewport.MaxDepth = 1.0f;
	DirectXDevice::viewport.MinDepth = 0.0f;

	//D3D12_RECT scissorrect{};
	DirectXDevice::scissorrect.left = 0; // 切り抜き座標左
	DirectXDevice::scissorrect.right = DirectXDevice::scissorrect.left + Camera::window_width; // 切り抜き座標右
	DirectXDevice::scissorrect.top = 0; // 切り抜き座標上
	DirectXDevice::scissorrect.bottom = DirectXDevice::scissorrect.top + Camera::window_height; // 切り抜き座標下



}
void DirectXDevice::SetFence()
{
	HRESULT result;

	result = DirectXDevice::dev->CreateFence(DirectXDevice::fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&DirectXDevice::fence));

}



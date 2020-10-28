#include<Windows.h>
#ifdef _DEBUG
#include<iostream>
#endif
#include<d3d12.h>
#include <wrl.h>
#include<dxgi1_6.h>
#include<vector>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#include"Texture.h"
#include"PMDClass.h"
#include"Floor.h"
#include"Camera.h"
#include"Poly.h"
#include"Sprite.h"
#include"ObjFile.h"
#include"Tower.h"
#include"Block.h"
#include"DirectXDevice.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace std;
using namespace DirectX;

//@brief
// @param format
// @param
// @remark

void DebugOutputFormatString(const char* format)
{
#ifdef  _DEBUG
	//va_list valist;
	//va_start(valist, format);
	//printf(format, valist);
	//va_end(valist);

#endif 

}
const int window_width = 1280;
const int window_height = 720;
struct  Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};
Vertex vertices[4] =
{
	{{-0.4f,-0.7f,0},{0.0f,1.0f}},
{{-0.4f,0.7f,0},{0.0f,0.0f} },
{{0.4f,-0.7f,0} , { 1.0f,1.0f }},
{{0.4f,0.7f,0} , { 0.0f,0.0f }},

};

unsigned short indices[] = {
	0,1,2,
	2,1,3
};

D3D12_VIEWPORT viewport = {};
D3D12_RECT scissorrect = {};


void SetView_Scissor();
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

 #ifdef DEBUG
int main()
{
#else



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#endif 
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
	//デバッグレイヤーをオンに	
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

	WNDCLASSEX w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;
	w.lpszClassName = (L"DXSample");
	w.hInstance = GetModuleHandle(nullptr);
	RegisterClassEx(&w);

	RECT wrc = { 0,0,window_width,window_height };

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd = CreateWindow(w.lpszClassName,
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
	MSG msg{};

	DirectXDevice::DirectXDevice::dev = nullptr;
	IDXGIFactory6* dxgifactory = nullptr;
	IDXGISwapChain4* swapchain = nullptr;

#ifdef DEBUG/*

	ID3D12Debug* debugLayer;
	HRESULT dresult = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
	debugLayer->EnableDebugLayer();
	debugLayer->Release();
*/
#endif // DEBUG



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
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgifactory));
#endif 

	std::vector<IDXGIAdapter*>adapters;

	IDXGIAdapter* tmpAdapter = nullptr;
	for (int i = 0;
		dxgifactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
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

	ID3D12CommandAllocator* cmdAllocator = nullptr;
	DirectXDevice::cmdList = nullptr;

	result = DirectXDevice::dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));
	result = DirectXDevice::dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator, nullptr,
		IID_PPV_ARGS(&DirectXDevice::cmdList));

	ID3D12CommandQueue* cmdQueue = nullptr;
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};

	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	result = DirectXDevice::dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};

	swapchainDesc.Width = window_width;
	swapchainDesc.Height = window_height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.Stereo = false;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;

	swapchainDesc.Scaling = DXGI_SCALING_STRETCH;

	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	result = dxgifactory->CreateSwapChainForHwnd(
		cmdQueue,
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&swapchain
	);

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 2;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; 
	ID3D12DescriptorHeap* rtvHeaps = nullptr;

	result = DirectXDevice::dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);

	std::vector<ID3D12Resource*>backBuffers(swcDesc.BufferCount);

	D3D12_CPU_DESCRIPTOR_HANDLE handle =
		rtvHeaps->GetCPUDescriptorHandleForHeapStart();

	/*D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;*/

	for (int idx = 0; idx < swcDesc.BufferCount; ++idx)
	{
		result = swapchain->GetBuffer(idx, IID_PPV_ARGS(&backBuffers[idx]));


		//DirectXDevice::dev->CreateRenderTargetView(backBuffers[idx], &rtvDesc, handle);


		DirectXDevice::dev->CreateRenderTargetView(backBuffers[idx], nullptr, handle);

		handle.ptr += DirectXDevice::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	}

	Camera* c=new Camera();
	
	//今はここで生成とInitializeしてください
	SetView_Scissor();
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;
	Floor* floor = new Floor();

	floor->Initialize(DirectXDevice::dev, c);
	floor->SetPos(XMFLOAT3(0, -25, 0));
	floor->SetScale(XMFLOAT3(500, 500, 500));

	Floor* floor2 = new Floor();
	floor2->Initialize(DirectXDevice::dev, c);
	floor2->SetPos(XMFLOAT3(0, 0, 200));
	floor2->SetScale(XMFLOAT3(50, 50, 50));

	Floor* floor3 = new Floor();
	floor3->Initialize(DirectXDevice::dev, c);
	floor3->SetPos(XMFLOAT3(-100, 25, 300));
	floor3->SetScale(XMFLOAT3(50, 50, 50));

	PMDClass* pmd = new PMDClass();
	pmd->Initialize(DirectXDevice::dev,c);
	result = DirectXDevice::dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	Tower* tower = new Tower();
	tower->Initialize(DirectXDevice::dev);
	
	Block* b = new Block();
	b->Initialize();
	b->SetPos(XMFLOAT3(10, 50, 10));
	//ここまで
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			break;
		}

		
		UINT bbIdx = swapchain->GetCurrentBackBufferIndex();
		D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		rtvH.ptr += bbIdx * DirectXDevice::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);


		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrierDesc.Transition.Subresource = 0;

		barrierDesc.Transition.pResource = backBuffers[bbIdx];
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		DirectXDevice::cmdList->ResourceBarrier(1, &barrierDesc);



		D3D12_CPU_DESCRIPTOR_HANDLE dsvH = pmd->PMDdsvHeap->GetCPUDescriptorHandleForHeapStart();

		DirectXDevice::cmdList->OMSetRenderTargets(1, &rtvH, true, &dsvH);
		DirectXDevice::cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		float clearColor[] = { 1.0f,1.0f,1.0f,0.0f };
		
		DirectXDevice::cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);


	
		DirectXDevice::cmdList->RSSetViewports(1, &viewport);

		DirectXDevice::cmdList->RSSetScissorRects(1, &scissorrect);

		//tex->Update();
		
	

	floor->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
		floor->Update();


	

		tower->Draw(DirectXDevice::cmdList);
		tower->Update();
	
		b->Draw();
		b->Update();

		////////

		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		DirectXDevice::cmdList->ResourceBarrier(1, &barrierDesc);

		DirectXDevice::cmdList->Close();

		ID3D12CommandList* cmdLists[] = { DirectXDevice::cmdList };
		cmdQueue->ExecuteCommandLists(1, cmdLists);

		
		cmdQueue->Signal(fence, ++fenceVal);
		while (fence->GetCompletedValue() != fenceVal)
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}


		cmdAllocator->Reset();
		DirectXDevice::cmdList->Reset(cmdAllocator, nullptr);

		swapchain->Present(1, 0);
	}
	UnregisterClass(w.lpszClassName, w.hInstance);


	DebugOutputFormatString("Show window test.");
	getchar();
	return 0;


}

void SetView_Scissor()
{
	
	viewport.Width = window_width;
	viewport.Height = window_height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;


	//D3D12_RECT scissorrect{};
	scissorrect.left = 0; // 切り抜き座標左
	scissorrect.right = scissorrect.left + window_width; // 切り抜き座標右
	scissorrect.top = 0; // 切り抜き座標上
	scissorrect.bottom = scissorrect.top + window_height; // 切り抜き座標下



}
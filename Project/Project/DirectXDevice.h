
#pragma once
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
#include<DirectXTex.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace std;
using namespace DirectX;


static class DirectXDevice
{
private:

public:
	static void Initialize();
	static void Update();
	static void CreateGameWindow();
	static void CreateDevice();
	static void CreatecmdList_Queue();
	static void CreateSwapchain();
	static void CreateRTV();
	static void CreateDsv();
	static void SetView_Scissor();
	static void SetFence();
	static void CollisionUpdate();
	static ID3D12GraphicsCommandList* cmdList;
	static ID3D12Device* dev;
	static IDXGIFactory6* dxgifactory;
	static IDXGISwapChain4* swapchain;
	static D3D12_VIEWPORT viewport;
	static D3D12_VIEWPORT viewport2;

	static D3D12_RECT scissorrect;
	static ID3D12CommandAllocator* cmdAllocator;
	static ID3D12CommandQueue* cmdQueue;
	static HWND hwnd;
	static ID3D12DescriptorHeap* rtvHeaps;
	static std::vector<ID3D12Resource*>backBuffers;
	static ID3D12DescriptorHeap* dsvHeap;
	static ID3D12Resource* depthBuff;
	static ID3D12Fence* fence;
	static UINT64 fenceVal;
	static WNDCLASSEX w;
	static float clearColor[];
	static D3D12_CPU_DESCRIPTOR_HANDLE rtvH;
};
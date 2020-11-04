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


	MSG msg{};
	DirectXDevice::Initialize();
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
		DirectXDevice::Update();

	}
	UnregisterClass(DirectXDevice::w.lpszClassName, DirectXDevice::w.hInstance);


	DebugOutputFormatString("Show window test.");
	getchar();
	return 0;


}
#pragma once
#include<Windows.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInput のバージョン指定
#include <dinput.h> 
#include<DirectXMath.h>
class Input
{
public:
	Input();
	~Input();
	void Initialize();
	void Update();
	bool PushKey(int key);
	bool TriggerKey(int key);
	bool PushMouseLeft();
	bool PushMouseRight();
	DirectX::XMFLOAT3 GetMousePos();
	
private:
	IDirectInputDevice8* devkeyboard = nullptr;
	IDirectInputDevice8* devmouse = nullptr;
//	LPDIRECTINPUTDEVICE devmouse = nullptr;

	DIMOUSESTATE dimousestate;
	BYTE key[256] = {};
	IDirectInput8* dinput = nullptr;
	bool trigger1 = false;
	bool trigger2 = false;

};


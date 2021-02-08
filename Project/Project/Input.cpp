#include "Input.h"
#include"DirectXDevice.h"
Input::Input()
{
}

Input::~Input()
{
	devkeyboard->Release();
	devmouse->Release();
	dinput->Release();
//	delete(devkeyboard);

//	delete(dinput);
}

void Input::Initialize()
{
	if (devkeyboard == nullptr)
	{
		HRESULT result;
		result = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

		result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
		result = dinput->CreateDevice(GUID_SysMouse, &devmouse, NULL);

		result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
		result = devmouse->SetDataFormat(&c_dfDIMouse); // 標準形式
		result = devmouse->SetCooperativeLevel(FindWindow(DirectXDevice::w.lpszClassName, nullptr), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	}
	
}

void Input::Update()
{
	HRESULT result;
	dimousestate.rgbButtons[0] = 0;
	result = devkeyboard->Acquire();
	result = devmouse->Acquire();
	//devmouse->Poll();
	
	result = devkeyboard->GetDeviceState(sizeof(key), key);
	result = devmouse->GetDeviceState(sizeof(DIMOUSESTATE), &dimousestate);
	

}

bool Input::PushKey(int keynum)
{
	if (keynum < 0)return false;
	if (keynum > 255)return false;
	if (key[keynum])
	{
		return true;
	}
	return false;
}

bool Input::TriggerKey(int keynum)
{

	if (keynum < 0)
	{
		trigger1 = false;
		return false;
	}
	if (keynum > 255) 
	{
		trigger1 = false;
		return false;
	}
	if (key[keynum] && !trigger1)
	{
		trigger1 = true;
		return true;
	}
	else if (key[keynum] && trigger1)
	{
		trigger1 = true;
		return false;
	}
	trigger1 = false;
	return false;
}

bool Input::PushMouseLeft()
{
	if (dimousestate.rgbButtons[0] & (0x80))
	{
		return true;


	}
	else
	{
		return false;

	}
}

bool Input::PushMouseRight()
{
	if (dimousestate.rgbButtons[1] & (0x80))
	{
		return true;
	}
	else
	{
		return false;
	}
}

DirectX::XMFLOAT3 Input::GetMousePos()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(FindWindow(DirectXDevice::w.lpszClassName, nullptr), &p);
	return { static_cast<float>(p.x),static_cast<float>(p.y),0 };
}

#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::Initialize()
{
	HRESULT result;
	IDirectInput8* dinput = nullptr;
	result = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	devkeyboard = nullptr;
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // •W€Œ`®
}

void Input::Update()
{
	HRESULT result;
	result = devkeyboard->Acquire();

	
	result = devkeyboard->GetDeviceState(sizeof(key), key);
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



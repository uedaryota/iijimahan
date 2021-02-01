#include "Button.h"
#include"DirectXDevice.h"

Button::Button()
{

}

Button::~Button()
{

}

void Button::Initialize()
{
	for (int i = 0; i <= ButtonCount - 1; i++)
	{
		buttons[i] = new ObjFile();
		buttons[i]->Initialize();
		buttons[i]->LoadObj("Button");
		buttons[i]->SetScale(XMFLOAT3(8, 8, 8));
	}
	buttons[0]->SetPos(XMFLOAT3(-37, 0, -70));
	buttons[1]->SetPos(XMFLOAT3(-67, 0, -164));
	buttons[2]->SetPos(XMFLOAT3(56, 0, -86));
	buttons[3]->SetPos(XMFLOAT3(-13, 0, 50));
	buttons[4]->SetPos(XMFLOAT3(75, 0, 83));
	buttons[5]->SetPos(XMFLOAT3(168, 0, 130));
	buttons[6]->SetPos(XMFLOAT3(171, 0, 45));

}

void Button::Update()
{
	for (int i = 0; i <= ButtonCount - 1; i++)
	{
		buttons[i]->Update();
	}
}

void Button::Draw()
{
	for (int i = 0; i <= ButtonCount - 1; i++)
	{
		buttons[i]->Draw(DirectXDevice::cmdList);
	}
}
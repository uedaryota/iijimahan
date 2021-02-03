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
	for (int i = 0; i <= ButtonCount; i++)
	{
		buttons[i] = new ObjFile();
		buttons[i]->Initialize();
		buttons[i]->LoadObj("Button");
		buttons[i]->SetScale(XMFLOAT3(8, 8, 8));

		black_buttons[i] = new ObjFile();
		black_buttons[i]->Initialize();
		black_buttons[i]->LoadObj("Button_Black");
		black_buttons[i]->SetScale(XMFLOAT3(8, 8, 8));
	}
	buttons[0]->SetPos(XMFLOAT3(-37, 0, -70));
	buttons[1]->SetPos(XMFLOAT3(-67, 0, -164));
	buttons[2]->SetPos(XMFLOAT3(56, 0, -86));
	buttons[3]->SetPos(XMFLOAT3(-13, 0, 50));
	buttons[4]->SetPos(XMFLOAT3(75, 0, 83));
	buttons[5]->SetPos(XMFLOAT3(168, 0, 130));
	buttons[6]->SetPos(XMFLOAT3(171, 0, 45));
	black_buttons[0]->SetPos(XMFLOAT3(-76, 0, -54));
	black_buttons[1]->SetPos(XMFLOAT3(-106, 0, -149));
	black_buttons[2]->SetPos(XMFLOAT3(17, 0, -68));
	black_buttons[3]->SetPos(XMFLOAT3(-53, 0, 65));
	black_buttons[4]->SetPos(XMFLOAT3(35, 0, 100));
	black_buttons[5]->SetPos(XMFLOAT3(130, 0, 145));
	black_buttons[6]->SetPos(XMFLOAT3(131, 0, 60));

	costflag = false;
}

void Button::Update()
{
	for (int i = 0; i <= ButtonCount; i++)
	{
		if (costflag)
		{
			buttons[i]->Update();
		}
		else
		{
			black_buttons[i]->Update();
		}
	}
}

void Button::Draw()
{
	for (int i = 0; i <= ButtonCount; i++)
	{
		if (costflag)
		{
			buttons[i]->Draw(DirectXDevice::cmdList);
		}
		else
		{
			black_buttons[i]->Draw(DirectXDevice::cmdList);
		}
	}
}
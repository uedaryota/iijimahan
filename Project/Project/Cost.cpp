#include "Cost.h"

void Cost::Initialize()
{
	time = 0;
	cost = 0;

	//�e�L�X�g�̓ǂݍ���
	Sprite::LoadTexture(TextNumber, L"Resources/debugfont.png");

	//�e�L�X�g������
	text = Text::GetInstance();
	text->Initialize(TextNumber);
}

void Cost::Update()
{
	if (cost <= maxcost)
	{
		if (time % costUpTime == 0)
		{
			cost++;
		}
	}
	if (cost > maxcost)
	{
		cost = maxcost;
	}
	time++;
}

void Cost::Draw()
{
	Sprite::PreDraw(DirectXDevice::cmdList);
	text->Print(to_string(cost), 0, 0, 5);
	text->DrawAll(DirectXDevice::cmdList);
	Sprite::PostDraw();
}

int Cost::GetCost()
{
	return cost;
}
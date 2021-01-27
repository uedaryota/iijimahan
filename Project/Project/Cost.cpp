#include "Cost.h"

void Cost::Initialize()
{
	time = 0;
	cost = 0;

	Sprite::LoadTexture(5, L"img/costback.png");

	//�e�L�X�g������
	text = Text::GetInstance();
	text->Initialize(TextNumber);
	sprite = Sprite::Create(5, XMFLOAT2(0, 0));
	sprite->SetSize(XMFLOAT2( 350, 50 ));
}

void Cost::Update()
{
	if (cost == maxcost) return;
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
	sprite->Draw();
	if (cost < 10)
	{
		text->Print("COST  " + to_string(cost) + " / " + to_string(maxcost), 0, 0, 3);
	}
	if (cost >= 10 && cost < 100)
	{
		text->Print("COST " + to_string(cost) + " / " + to_string(maxcost), 0, 0, 3);
	}
	if (cost >= 100)
	{
		text->Print("COST" + to_string(cost) + " / " + to_string(maxcost), 0, 0, 3);
	}
	text->DrawAll(DirectXDevice::cmdList);
	Sprite::PostDraw();
}

int Cost::GetCost()
{
	return cost;
}
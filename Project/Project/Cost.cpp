#include "Cost.h"

void Cost::Initialize()
{
	costUpTime = 30;
	time = 0;
	cost = 0;

	Sprite::LoadTexture(5, L"img/costback.png");

	//テキスト初期化
	text = Text::GetInstance();
	text->Initialize(0);
	redtext = RedText::GetInstance();
	redtext->Initialize(10);
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
		redtext->Print("COST  " + to_string(cost) + " / " + to_string(maxcost), 10, 0, 3);
	}
	if (cost >= 10 && cost < 20)
	{
		redtext->Print("COST " + to_string(cost) + " / " + to_string(maxcost), 10, 0, 3);
	}
	if (cost >= 20 && cost < 100)
	{
		text->Print("COST " + to_string(cost) + " / " + to_string(maxcost), 10, 0, 3);
	}
	if (cost >= 100)
	{
		text->Print("COST" + to_string(cost) + " / " + to_string(maxcost), 10, 0, 3);
	}
	redtext->DrawAll(DirectXDevice::cmdList);
	Sprite::PostDraw();
}

int Cost::GetCost()
{
	return cost;
}
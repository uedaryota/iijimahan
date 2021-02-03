#include "Cost.h"

void Cost::Initialize()
{
	time = 0;
	cost = 0;

	Sprite::LoadTexture(5, L"img/costback.png");

	//テキスト初期化
	text = Text::GetInstance();
	text->Initialize(TextNumber);
	sprite = Sprite::Create(5, XMFLOAT2(0, 0));
	sprite->SetSize(XMFLOAT2( 350, 50 ));
	text->SetColor(XMFLOAT4(1, 0, 0, 1));
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

	costtrig = costflag;
	if (cost >= 20)
	{
		costflag = true;
	}
	else
	{
		costflag = false;
	}
	if (costflag != costtrig)
	{
		if (costflag)
		{
			text->SetColor(XMFLOAT4(1, 1, 1, 1));
		}
		else
		{
			text->SetColor(XMFLOAT4(1, 0, 0, 1));
		}
	}
}

void Cost::Draw()
{
	Sprite::PreDraw(DirectXDevice::cmdList);
	sprite->Draw();
	if (cost < 10)
	{
		text->Print("COST  " + to_string(cost) + " / " + to_string(maxcost), 10, 0, 3);
	}
	if (cost >= 10 && cost < 100)
	{
		text->Print("COST " + to_string(cost) + " / " + to_string(maxcost), 10, 0, 3);
	}
	if (cost >= 100)
	{
		text->Print("COST" + to_string(cost) + " / " + to_string(maxcost), 10, 0, 3);
	}
	text->DrawAll(DirectXDevice::cmdList);
	Sprite::PostDraw();
}

int Cost::GetCost()
{
	return cost;
}
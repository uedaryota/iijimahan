#include "Cost.h"

void Cost::Initialize()
{
	time = 0;
	cost = 0;
	sprite->Initialize();
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
	sprite->Update();
}

void Cost::Draw()
{

}

int Cost::GetCost()
{
	return cost;
}
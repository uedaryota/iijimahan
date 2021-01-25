#include "DefenderSpawn.h"

DefenderSpawn::DefenderSpawn()
{
}

DefenderSpawn::~DefenderSpawn()
{
}

void DefenderSpawn::Update()
{
	col->Update();
	if (battery != nullptr)
	{
		battery->Update();
	}
}

void DefenderSpawn::Draw()
{
	col->Draw();
	if (battery != nullptr)
	{
		battery->Draw();
	}
}

void DefenderSpawn::Initialize()
{
	col = new CircleCollision();
	col->Initialize();
	col = new CircleCollision();
	col->scale = 10;
	col->color = { 1,0,0,1 };
	costFlag = false;
	battery = new Battery();
	battery->Initialize();
	
}

void DefenderSpawn::CreateBattery()
{
	if (!battery->liveFlag&&!battery->MoveFlag)
	{
		battery->MoveFlag = true;
		battery->position.y = 200;

		costFlag = true;
	}

}

void DefenderSpawn::StopCreate()
{
	battery->MoveFlag = false;
	battery->liveFlag = false;
	battery->position.y = 0;

	costFlag = false;
}

void DefenderSpawn::SetPos(XMFLOAT3 pos)
{
	position = pos;
	col->position = position;
	battery->SetPos(position);
	//col->position.y += col->scale / 2;
}

bool DefenderSpawn::ReturnCostFlag()
{
	return costFlag;
}

void DefenderSpawn::CostFlagTrue()
{
	costFlag = true;
}

void DefenderSpawn::CostFlagFalse()
{
	costFlag = false;
}

int DefenderSpawn::ReturnUseCost()
{
	return usecost;
}


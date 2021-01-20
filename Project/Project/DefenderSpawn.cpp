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
	col->scale = 30;
	col->color = { 1,0,0,1 };

	battery = new Battery();
	battery->Initialize();
	
}

void DefenderSpawn::CreateBattery()
{
	battery->liveFlag = true;
}

void DefenderSpawn::SetPos(XMFLOAT3 pos)
{
	position = pos;
	col->position = position;
	battery->SetPos(position);
	//col->position.y += col->scale / 2;
}

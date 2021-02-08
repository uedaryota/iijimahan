#include "DefenderSpawn.h"

DefenderSpawn::DefenderSpawn()
{
}

DefenderSpawn::~DefenderSpawn()
{
	delete(battery);
	delete(col);
}

void DefenderSpawn::Update()
{
	//obj->SetPos(position);
	obj->Update();
	//obj2->SetPos(position);
	obj2->Update();
	col->Update();
	if (battery != nullptr)
	{
		battery->Update();
	}
}

void DefenderSpawn::Draw()
{

//	col->Draw();
	if (battery != nullptr)
	{
		battery->Draw();
	}


	if (possibleSet)
	{
		obj->Draw();
	}
	else
	{
		obj2->Draw();
	}
}

void DefenderSpawn::Initialize()
{
	obj->Initialize();
	obj->LoadObj("Button");
	obj->SetScale(XMFLOAT3(8, 8, 8));

	obj2->Initialize();
	obj2->LoadObj("Button_Black");
	obj2->SetScale(XMFLOAT3(8, 8, 8));

	//col = new CircleCollision();
	col->Initialize();
	col->scale = 10;
	col->color = { 1,0,0,1 };
	costFlag = false;
	possibleSet = false;
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
	obj->SetPos(position);
	obj->position.x += 3;
	obj->position.z += 20;

	//obj->position.y += 10;
	obj2->SetPos(obj->position);
	//obj2->position.y += 10;
	obj2->position.x -= 39;
	obj2->position.z += 16;

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


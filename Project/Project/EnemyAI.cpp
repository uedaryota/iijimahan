#include "EnemyAI.h"
#include"DirectXDevice.h"

EnemyAI::EnemyAI()
{
}

//EnemyAI::~EnemyAI()
//{
//	delete(pol);
//}

void EnemyAI::Initialize()
{
	pol->Initialize();
}



void EnemyAI::SetPos(XMFLOAT3 position)
{
	pol->position = position;
}

void EnemyAI::SetTarget(Tower tow)
{
	tower = tow;
}




float EnemyAI::GetPower()
{
	return Power;
}

void EnemyAI::SetPower(float x)
{
	Power = x;
}
EnemyAI EnemyAI::GetAI(EnemyAI ai)
{
	return ai;
}




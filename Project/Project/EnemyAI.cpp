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
	pol->Initialize(DirectXDevice::dev);
}



void EnemyAI::SetPos(XMFLOAT3 position)
{
	pol->position = position;
}

void EnemyAI::SetTarget(Tower tow)
{
	tower = tow;
}

float EnemyAI::GetHp()
{
	return Hp;
}

float EnemyAI::GetSpeed()
{
	return Speed;
}

float EnemyAI::GetPower()
{
	return Power;
}

EnemyAI EnemyAI::GetAI(EnemyAI ai)
{
	return ai;
}




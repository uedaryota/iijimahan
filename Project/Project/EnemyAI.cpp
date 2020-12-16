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

void EnemyAI::SetHp(float x)
{
	Hp = x;
}

float EnemyAI::GetSpeed()
{
	return Speed;
}

void EnemyAI::SetSpeed(float x)
{
	Speed = x;
	Speed;
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




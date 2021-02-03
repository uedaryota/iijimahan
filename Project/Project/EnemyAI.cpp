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





EnemyAI EnemyAI::GetAI(EnemyAI ai)
{
	return ai;
}




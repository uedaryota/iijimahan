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

XMFLOAT3 EnemyAI::UpdateAI()
{
	
	velocity.x = tower.GetPosition().x - pol->position.x*0.01f;
	velocity.y = tower.GetPosition().y - pol->position.y*0.01f;
	velocity.z = tower.GetPosition().z - pol->position.z*0.01f;
	return velocity;
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

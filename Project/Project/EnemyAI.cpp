#include "EnemyAI.h"
#include"DirectXDevice.h"

EnemyAI::EnemyAI()
{
}

EnemyAI::~EnemyAI()
{
	delete(pol);
}

void EnemyAI::Initialize()
{
	pol->Initialize(DirectXDevice::dev);
}

void EnemyAI::Update()
{
	XMFLOAT3 velocity;
	velocity.x = pol->position.x - tower.GetPosition().x;
	velocity.y = pol->position.y - tower.GetPosition().y;
	velocity.z = pol->position.z - tower.GetPosition().z;
	pol->position.x += velocity.x;
	pol->position.y += velocity.y;
	pol->position.z += velocity.z;
	pol->Update();
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

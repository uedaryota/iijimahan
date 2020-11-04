#include"Enemy.h"
#include"DirectXDevice.h"
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete(pol);
}

void Enemy::Initialize()
{
	pol->Initialize(DirectXDevice::dev);
	ai.Initialize();
	SetState();
}

void Enemy::Update()
{
	pol->Update();
	//vel = //ai.UpdateAI();
	PositionUpdate();
}



void Enemy::Draw()
{
	pol->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
}

void Enemy::SetPos(XMFLOAT3 position)
{
	pol->position = position;
}

void Enemy::SetScale(XMFLOAT3 scale)
{
	pol->scale = scale;
}

void Enemy::Install(EnemyAI Ai)
{
	ai = Ai;
}

void Enemy::SetState()
{
	Hp = ai.GetHp();
	Speed = ai.GetSpeed();
	Power = ai.GetPower();
}

void Enemy::PositionUpdate()
{
	if (pol->position.x > 0) {
		pol->position.x = pol->position.x + vel.x;
	}
	pol->position.y = pol->position.y + vel.y;
	if (pol->position.z > 0)
	{
		pol->position.z = pol->position.z + vel.z;
	}
}


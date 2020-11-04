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
	SetState();
}

void Enemy::Update()
{
	pol->Update();
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


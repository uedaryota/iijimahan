#include "EnemyManeger.h"

void EnemyManeger::Initialize()
{
	boxcount = 0;
}

void EnemyManeger::Add(Enemy* enemy)
{
	enemybox[boxcount] = enemy;
	enemybox[boxcount]->Initialize();
	enemybox[boxcount]->SetScale(XMFLOAT3(2, 2, 2));
	enemybox[boxcount]->SetPos(XMFLOAT3(60, 60, 60));
	boxcount++;
}

void EnemyManeger::Create()
{
	for (int i = 0; i < boxcount; i++)
	{
		enemybox[i]->Initialize();
		enemybox[i]->SetScale(XMFLOAT3(2, 2, 2));
		enemybox[i]->SetPos(XMFLOAT3(60, 60, 60));
	}
}

void EnemyManeger::Destory()
{
}

void EnemyManeger::Draw()
{
	for (int i = 0; i < boxcount; i++)
	{
		enemybox[i]->Draw();
	}
}

void EnemyManeger::Update()
{
	for (int i = 0; i < boxcount; i++)
	{
		enemybox[i]->Update();
	}
}

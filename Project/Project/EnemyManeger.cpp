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

///<summary>
///第1第2ポイントまとめての移動目標の全体設定
///<param name="anc1">第1移動地点</param>
///<param name="anc2">第2移動地点</param>
///</summary>
void EnemyManeger::AncerSet(XMFLOAT3 anc1, XMFLOAT3 anc2)
{
	for (int i = 0; i < boxcount; i++)
	{
		enemybox[i]->SetAncer1(anc1);
		enemybox[i]->SetAncer2(anc2);
	}
}

///<summary>
///第1ポイントの全体設定
///<param name="anc1">第1移動地点</param>
///</summary>
void EnemyManeger::FirstAncerSet(XMFLOAT3 anc1)
{
	for (int i = 0; i < boxcount; i++)
	{
		enemybox[i]->SetAncer1(anc1);
	}
}

///<summary>
///第2ポイントの全体設定
///<param name="anc2">第2移動地点</param>
///</summary>
void EnemyManeger::SecondAncerSet(XMFLOAT3 anc2)
{
	for (int i = 0; i < boxcount; i++)
	{
		enemybox[i]->SetAncer2(anc2);
	}
}

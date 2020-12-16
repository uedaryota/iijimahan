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
///��1��2�|�C���g�܂Ƃ߂Ă̈ړ��ڕW�̑S�̐ݒ�
///<param name="anc1">��1�ړ��n�_</param>
///<param name="anc2">��2�ړ��n�_</param>
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
///��1�|�C���g�̑S�̐ݒ�
///<param name="anc1">��1�ړ��n�_</param>
///</summary>
void EnemyManeger::FirstAncerSet(XMFLOAT3 anc1)
{
	for (int i = 0; i < boxcount; i++)
	{
		enemybox[i]->SetAncer1(anc1);
	}
}

///<summary>
///��2�|�C���g�̑S�̐ݒ�
///<param name="anc2">��2�ړ��n�_</param>
///</summary>
void EnemyManeger::SecondAncerSet(XMFLOAT3 anc2)
{
	for (int i = 0; i < boxcount; i++)
	{
		enemybox[i]->SetAncer2(anc2);
	}
}

void EnemyManeger::ReAncerSet(XMFLOAT3 anc1, XMFLOAT3 anc2)
{
	for (int i = 0; i < boxcount; i++)
	{
		enemybox[i]->ActionRiset();
		enemybox[i]->SetAncer1(anc1);
		enemybox[i]->SetAncer2(anc2);
	}
}

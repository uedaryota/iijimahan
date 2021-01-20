#include "EnemyManeger.h"

void EnemyManeger::Initialize()
{
	boxcount = 0;
}

void EnemyManeger::Add(Enemy* enemy)
{
	//単体で生成しているところ
	enemybox[boxcount] = enemy;
	enemybox[boxcount]->Initialize();
	enemybox[boxcount]->SetScale(XMFLOAT3(2, 2, 2));
	enemybox[boxcount]->SetPos(XMFLOAT3(60, 60, 60));
	boxcount++;
}

void EnemyManeger::Add2(XMFLOAT3 x)
{
	//連続で生成しているところ：基本はこっちで生成している
	if (Cnt > 200)
	{
		return;
	}
	else
	{
			enemybox[boxcount] = spawn->enemyamount();
			enemybox[boxcount]->Initialize();
			enemybox[boxcount]->SetScale(XMFLOAT3(10, 10, 10));
			enemybox[boxcount]->SetPos(x);
			enemybox[boxcount]->ActionRiset();
			enemybox[boxcount]->state = move1;
			boxcount++;
			Cnt++;
	}

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

void EnemyManeger::Draw(ID3D12GraphicsCommandList * cmdList)
{
	for (int i = 0; i < boxcount; i++)
	{
		enemybox[i]->Draw(cmdList);
	}
}

void EnemyManeger::Update()
{
	for (int i = 0; i < boxcount; i++)
	{
		enemybox[i]->Update();
	
	}
	//Serch();
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

void EnemyManeger::ReAncerSet(XMFLOAT3 anc1, XMFLOAT3 anc2)
{
	for (int i = 0; i < boxcount; i++)
	{
		
		enemybox[i]->SetAncer1(anc1);
		enemybox[i]->SetAncer2(anc2);
	}
}

bool EnemyManeger::Serch()
{
	float hitX = 0.5f;
	float hitZ = 0.5f;

	for (int i = 0; i < boxcount; i++)
	{
		for (int x = 0; x < boxcount; x++)
		{
			if (enemybox[i]->GetPosition().x - enemybox[x]->GetPosition().x == 0 && enemybox[i]->GetPosition().z - enemybox[x]->GetPosition().z == 0)
			{
			}
			else if (abs(enemybox[i]->GetPosition().x - enemybox[x]->GetPosition().x) > hitX)
			{
			}
			else if (abs(enemybox[i]->GetPosition().z - enemybox[x]->GetPosition().z) > hitZ)
			{
			}
			else
			{
				enemybox[i]->state = Stay;
			}
		}
	}
	return false;
}

void EnemyManeger::SetTowerEnemy(Tower* tow)
{
	for (int i = 0; i < boxcount; i++)
	{
		enemybox[i]->SetTower(tow);
	}
}

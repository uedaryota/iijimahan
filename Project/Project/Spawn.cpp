#include "Spawn.h"
#include"Input.h"
Spawn::Spawn()
{

}

Spawn::~Spawn()
{
	delete(obj);
}

void Spawn::Initialize(ID3D12Device* dev)
{
	Cnt = 0;
	obj->Initialize();
	obj->LoadObj("Enemy_Base");
	obj->position.y = 10;
	obj->SetScale({ 10,10,10 });
	obj->rotation.y =4.6;
}

void Spawn::Update()
{	
	obj->Update();
	for (int i = 0; i < amount; i++)
	{
		time++;
		if (time > 60 * timer)
		{
			enemy[i - 1]=new Enemy();
			//manager->Add(enemy[i - 1]);
			time = 0;
		}
	}
}

void Spawn::Draw(ID3D12GraphicsCommandList * cmdList)
{
	obj->Draw(cmdList);
}


XMFLOAT3 Spawn::GetPosition()
{
	XMFLOAT3 position;
	position = obj->position;
	return position;
}

void Spawn::SetPoisition(XMFLOAT3 position)
{
	obj->position = position;
}

void Spawn::SetSpawn(int x, int y)
{
	amount = x;
	backamount = &amount;
	timer = y;
}

Enemy* Spawn::enemyamount()
{
	Cnt = Cnt + 1;
	enemy[Cnt] = new Enemy();
	//amount = amount - 1;
	return enemy[Cnt];
}

int Spawn::GetAmount()
{
	amount = *backamount;
	return int(amount);
}


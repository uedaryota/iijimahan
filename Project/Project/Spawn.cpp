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
	texPos = { 0,Camera::window_height - 100,0 };
	tex->Initialize();
	tex->position = { texPos.x, texPos.y / Camera::window_height, 0 };
	tex->scale = { texSize.x / Camera::window_width,texSize.y / Camera::window_height,100.0f };

	obj->Initialize();
	obj->LoadObj("Enemy_Base");
	obj->position.y = 10;
	obj->SetScale({ 30,30,30 });
}

void Spawn::Update()
{
	if (stop)
	{
		return;
	}	
	
	
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
	tex->position = { (texPos.x - (texSize.x / 2)) / Camera::window_width,
		texPos.y / Camera::window_height, 0 };//プロジェクションView行列掛けてないのでここで
	tex->scale = { texSize.x / Camera::window_width,texSize.y / Camera::window_height,100.0f };//同文
	obj->Update();
	tex->Update();
}

void Spawn::Draw(ID3D12GraphicsCommandList * cmdList)
{
	obj->Draw(cmdList);
	tex->Draw();
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

void Spawn::Stop()
{
	if (stop) 
	{
		stop = false;
	}
	else
	{
		stop = true;
	}
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


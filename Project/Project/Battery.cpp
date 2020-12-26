#include "Battery.h"
#include"DirectXDevice.h"
Battery::Battery()
{
	
}

Battery::~Battery()
{
	delete(obj);
}

void Battery::Update()
{
	obj->Update();
	input->Update();
	if (input->TriggerKey(DIK_T))
	{
		Shot();
	}
	if (bulletList.size() != 0)
	{
		for (int a = 0; a < bulletList.size(); a++)
		{
			bulletList[a]->Update();
		}
	}
}

void Battery::Draw()
{
	obj->Draw(DirectXDevice::cmdList);
	if (bulletList.size() != 0)
	{
		for (int a = 0; a < bulletList.size(); a++)
		{
			bulletList[a]->Draw();
		}
	}
}

void Battery::Initialize()
{
		obj = new ObjFile();
		input = new Input();
		input->Initialize();
	obj->Initialize();
	obj->LoadObj("Gun_All");
	col = new CircleCollision();
	col->length = 1000;
	SetScale({ 10,10,10 });
	SetPos({ 100, 0, 100 });
}

void Battery::SetTarget(XMFLOAT3* targetpos)
{
	this->targetPos = targetpos;
}

void Battery::SetPos(XMFLOAT3 pos)
{
	mainPos = pos;
	obj->SetPos(mainPos);
	col->position = mainPos;
}

void Battery::SetScale(XMFLOAT3 scale)
{
	obj->SetScale(scale);
}

void Battery::Shot()
{
	Bullet* b = new Bullet();
	b->Initialize();
	b->SetPos(mainPos);
	b->targetPos = targetPos;
	bulletList.push_back(b);
}

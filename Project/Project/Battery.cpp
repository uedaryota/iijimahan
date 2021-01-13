#include "Battery.h"
#include"DirectXDevice.h"
Battery::Battery()
{
	
}

Battery::~Battery()
{
	delete(obj);
	delete(input);
}

void Battery::Update()
{
	
	obj->Update();
	input->Update();
	col->Update();
	clickcol->Update();

	interval--;
	
	if (interval <= 0 && targetPos != nullptr)
	{
		interval = startval;
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
	//col->Draw();
	clickcol->Draw();
}

void Battery::Initialize()
{
	obj = new ObjFile();
	input = new Input();
	input->Initialize();
	obj->Initialize();
	obj->LoadObj("Rhino");
	col = new CircleCollision();
	col->scale = 100;
	clickcol = new CircleCollision();
	clickcol->scale = 30;
	clickcol->color = { 1,0,0,0.5 };
	//cloickcol->color={}
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
//	col->position.z += 10;
	clickcol->position = col->position;
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

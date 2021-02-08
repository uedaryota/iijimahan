#include "Battery.h"
#include"DirectXDevice.h"

Battery::Battery()
{
	
}

Battery::~Battery()
{
	delete(obj);
	obj = nullptr;
	delete(input);
	input = nullptr;
	delete(col);
	col = nullptr;
	delete(clickcol);
	clickcol = nullptr;
}

void Battery::Update()
{
	if (MoveFlag && !liveFlag)
	{
		if (position.y > 0)
		{
			position.y -= 3;
			SetPos(position);
			obj->Update();
			input->Update();
			col->Update();
		}
		else
		{
			liveFlag = true;
			position.y = 0;
			sound->PlayStart();
		}
		
	}
	if (liveFlag)
	{
		SetPos(position);
		obj->rotation = rotation;
		obj->Update();
		input->Update();
		col->Update();
		clickcol->Update();
		interval--;
		Setdir();
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
}

void Battery::Draw()
{
	//MoveFlag = true;
	if (MoveFlag)
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
	//	clickcol->Draw();
	}
}

void Battery::Initialize()
{
	input->Initialize();
	obj->Initialize();
	obj->LoadObj("Rhino");
	col->Initialize();
	col->scale = 150;
	clickcol->Initialize();
	clickcol->scale = 10;
	clickcol->color = { 1,0,0,0.5 };
	//cloickcol->color={}
	SetScale({ 10,10,10 });
	int bulletCount = 3;
	if (bulletList.size() == 0)
	{
		for (int a = 0; a < bulletCount; a++)
		{
			Bullet* b = new Bullet();
			bulletList.push_back(b);
		}
	}
	for (auto b : bulletList)
	{
		b->Initialize();
		b->SetPos(position);
		b->Reset();
	}
	liveFlag = false;
	MoveFlag = false;
	//SetPos({ 100, 0, 100 });
	sound->Initialize();
	sound->LoadFile(L".\\Resources\\clap.mp3");
}

void Battery::SetTarget(XMFLOAT3* targetpos)
{
	this->targetPos = targetpos;
}

void Battery::SetPos(XMFLOAT3 pos)
{
	position = pos;
	obj->SetPos(position);
	col->position = position;
//	col->position.z += 10;
	clickcol->position = col->position;
}

void Battery::SetScale(XMFLOAT3 scale)
{
	obj->SetScale(scale);
}

void Battery::Shot()
{
	for (int a = 0; a < bulletList.size(); a++)
	{
		if (!bulletList[a]->liveFlag)
		{
			bulletList[a]->targetPos = targetPos;
			bulletList[a]->liveFlag = true;
			bulletList[a]->SetPos(position);
			return;
		}
	}
}

void Battery::Break()
{
	liveFlag = false;
}

void Battery::Setdir()
{
	if (targetPos != nullptr)
	{
		float PI = 3.1415f;
		XMFLOAT3 dir = { targetPos->x - position.x, targetPos->y - position.y, targetPos->z - position.z };
		dir.y = 0;
		float len = sqrtf(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);
		dir = { dir.x / len, dir.y / len, dir.z / len };
		float angle = atan2f(dir.x, dir.z);
		rotation.y = angle + PI / 2;
		//angle = angle * (180 / PI);
	}

}

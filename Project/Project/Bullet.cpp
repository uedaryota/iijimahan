#include "Bullet.h"
#include"DirectXDevice.h"
Bullet::Bullet()
{
	Initialize();
}

Bullet::~Bullet()
{
	delete(obj);
	delete(col);
}

void Bullet::Update()
{
	if (targetPos != nullptr&&liveFlag)
	 {
		XMFLOAT3 v = { targetPos->x - position.x, targetPos->y - position.y, targetPos->z - position.z };
		float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
		v = { v.x / len * speed, v.y / len * speed, v.z / len * speed };
		velocity = v;
		position = { position.x + velocity.x,position.y + velocity.y,position.z + velocity.z };
		obj->SetPos(position);
		col->position = position;
		obj->Update();
	}

}

void Bullet::Draw()
{
	if (liveFlag)
	{
		obj->Draw(DirectXDevice::cmdList);
	}
}

void Bullet::Initialize()
{
	
	
	obj->Initialize();
	obj->SetScale({ 10,10,10 });

	obj->LoadObj("ball");
	//col = new CircleCollision();
	col->Initialize();
	col->scale = 10;
}

void Bullet::SetVelocity(XMFLOAT3 velocity)
{

	this->velocity = velocity;
}

void Bullet::SetPos(XMFLOAT3 pos)
{
	position = pos;
	obj->SetPos(position);
	col->position = position;
}

void Bullet::Reset()
{
	liveFlag = false;
	targetPos = nullptr;
}

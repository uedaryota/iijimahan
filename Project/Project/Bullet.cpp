#include "Bullet.h"
#include"DirectXDevice.h"
Bullet::Bullet()
{
	Initialize();
}

Bullet::~Bullet()
{
	delete(obj);
	
}

void Bullet::Update()
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

void Bullet::Draw()
{
	obj->Draw(DirectXDevice::cmdList);
}

void Bullet::Initialize()
{
	
	obj = new ObjFile();
	obj->Initialize();
	obj->SetScale({ 10,10,10 });

	obj->LoadObj("ball");
	col = new CircleCollision();
	col->length = 10;
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

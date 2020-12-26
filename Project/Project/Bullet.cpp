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
	position = { position.x + velocity.x,position.y + velocity.y,position.z + velocity.z };
	obj->SetPos(position);
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
	obj->SetScale({ 100,100,100 });

	obj->LoadObj("triangle_mat");
}

void Bullet::SetVelocity(XMFLOAT3 velocity)
{
	this->velocity = velocity;
}

void Bullet::SetPos(XMFLOAT3 pos)
{
	position = pos;
	obj->SetPos(position);
}

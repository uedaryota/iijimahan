#include "Sphere.h"
#include "Input.h"

Sphere::Sphere()
{

}

Sphere::~Sphere()
{
	delete(obj);
}

void Sphere::Initialize(ID3D12Device* dev)
{
	obj->Initialize();
	obj->LoadObj("sphere");
	obj->position.y = 50;
}

void Sphere::Update()
{
	obj->Update();
}

void Sphere::Draw(ID3D12GraphicsCommandList * cmdList)
{
	obj->Draw(cmdList);
}

void Sphere::SetPos(XMFLOAT3 pos)
{
	position = pos;
	obj->SetPos(position);
}

void Sphere::SetRot(XMFLOAT3 rot)
{
	rotation = rot;
	obj->SetRotate(rotation);
}

void Sphere::SetScale(XMFLOAT3 scale)
{
	this->scale = scale;
	obj->SetScale(this->scale);
}
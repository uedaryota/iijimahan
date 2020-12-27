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
	obj->Initialize(dev);
	obj->LoadObj("sphere");
	obj->position.y = 10;
}

void Sphere::Update()
{
	obj->Update();
}

void Sphere::Draw(ID3D12GraphicsCommandList * cmdList)
{
	obj->Draw(cmdList);
}
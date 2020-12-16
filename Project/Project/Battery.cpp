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
}

void Battery::Draw()
{
	obj->Draw(DirectXDevice::cmdList);
}

void Battery::Initialize()
{
	obj = new ObjFile();
	obj->Initialize(DirectXDevice::dev);
	obj->LoadObj("Gun_All");
	SetScale({ 10,10,10 });
	SetPos({ 100, 0, 100 });
}

void Battery::SetPos(XMFLOAT3 pos)
{
	obj->SetPos(pos);
}

void Battery::SetScale(XMFLOAT3 scale)
{
	obj->SetScale(scale);
}

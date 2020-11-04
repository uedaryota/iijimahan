#include "Stage.h"
#include"DirectXDevice.h"
Stage::Stage()
{

}

void Stage::Initialize()
{
	floor1->Initialize(DirectXDevice::dev);
	floor2->Initialize(DirectXDevice::dev);
	floor3->Initialize(DirectXDevice::dev);

	floor2->rotation = XMFLOAT3(0, XM_PIDIV2, 0);
	floor3->rotation = XMFLOAT3(0, 0, XM_PIDIV2);

	floor2->scale = XMFLOAT3(100, 30, 100);
	floor3->scale = XMFLOAT3(30, 100, 100);

	floor2->position = XMFLOAT3(100, 0, 0);
	floor3->position = XMFLOAT3(0, 0,100);

	
}
void Stage::Update()
{
	floor1->Update();
	floor2->Update();
	floor3->Update();

}

void Stage::Draw()
{
	floor1->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
	floor2->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
	floor3->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
}

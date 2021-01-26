#include "Stage2D.h"
#include"DirectXDevice.h"
#include"Collision.h"
Stage2D::Stage2D()
{

}

Stage2D::~Stage2D()
{
}

void Stage2D::Initialize()
{

	input = new Input();
	input->Initialize();
	floor = new Floor();
	floor->Initialize(DirectXDevice::dev);
	floor->SetScale({ 200, 200, 200 });

	floor2 = new Floor();
	floor2->Initialize(DirectXDevice::dev);	
	floor2->ResetTex(L"img/TDmap_Back.png", DirectXDevice::dev);
	floor2->position.y -= 0.1;
	floor2->SetScale({ 700, 700, 700 });

	rock->Initialize();
	rock->LoadObj("Rock");
	rock->SetScale(XMFLOAT3(20, 20, 20));
	rock->SetPos(XMFLOAT3(130, 0, -50));

	rock2->Initialize();
	rock2->LoadObj("Rock_02");
	rock2->SetScale(XMFLOAT3(20, 20, 20));
	rock2->SetPos(XMFLOAT3(170, 0, -150));

	rock3->Initialize();
	rock3->LoadObj("Rock");
	rock3->SetScale(XMFLOAT3(20, 20, 20));
	rock3->SetPos(XMFLOAT3(-110, 0, 30));

	rock4->Initialize();
	rock4->LoadObj("Rock_02");
	rock4->SetScale(XMFLOAT3(20, 20, 20));
	rock4->SetPos(XMFLOAT3(-140, 0, 130));

	rock5->Initialize();
	rock5->LoadObj("Rock");
	rock5->SetScale(XMFLOAT3(20, 20, 20));
	rock5->SetPos(XMFLOAT3(-30, 0, 130));

	rock6->Initialize();
	rock6->LoadObj("Rock");
	rock6->SetScale(XMFLOAT3(20, 20, 20));
	rock6->SetPos(XMFLOAT3(30, 0, -180));
}
void Stage2D::CreateStage(int count)
{

}
void Stage2D::Update()
{
	floor->Update();
	floor2->Update();

	input->Update();
	CameraMove();
	rock->Update();
	rock2->Update();
	rock3->Update();
	rock4->Update();
	rock5->Update();
	rock6->Update();
}

void Stage2D::Draw()
{
	floor->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
	floor2->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
	rock->Draw(DirectXDevice::cmdList);
	rock2->Draw(DirectXDevice::cmdList);
	rock3->Draw(DirectXDevice::cmdList);
	rock4->Draw(DirectXDevice::cmdList);
	rock5->Draw(DirectXDevice::cmdList);
	rock6->Draw(DirectXDevice::cmdList);
}
void Stage2D::CameraMove()
{
	if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_A))
	{
		//カメラアングル変更
		Camera::ReturnCameraState()->eyeangleY += 0.03;
	}
	if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_D))
	{
		//カメラアングル変更
		Camera::ReturnCameraState()->eyeangleY -= 0.03;
	}
}

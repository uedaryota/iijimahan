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

	for (int i = 0; i <= rockcount * 2 / 3 - 1; i++)
	{
		rocks[i] = new ObjFile();
		rocks[i]->Initialize();
		rocks[i]->LoadObj("Rock");
		if (i % 2 == 0)
		{
			rocks[i]->SetScale(XMFLOAT3(30, 23, 31));
			rocks[i]->SetRotate(XMFLOAT3(10, 30, -10));
		}
		else if (i % 3 == 0)
		{
			rocks[i]->SetScale(XMFLOAT3(33, 25, 32));
			rocks[i]->SetRotate(XMFLOAT3(-10, 45, 10));
		}
		else
		{
			rocks[i]->SetScale(XMFLOAT3(25, 20, 28));
			rocks[i]->SetRotate(XMFLOAT3(0, 60, 0));
		}
	}
	for (int i = 16; i <= rockcount - 1; i++)
	{
		rocks[i] = new ObjFile();
		rocks[i]->Initialize();
		rocks[i]->LoadObj("Rock_02");
		if (i % 2 == 0)
		{
			rocks[i]->SetScale(XMFLOAT3(30, 23, 30));
			rocks[i]->SetRotate(XMFLOAT3(10, 30, -10));
		}
		else if (i % 5 == 0)
		{
			rocks[i]->SetScale(XMFLOAT3(33, 25, 32));
			rocks[i]->SetRotate(XMFLOAT3(-10, 45, 10));
		}
		else
		{
			rocks[i]->SetScale(XMFLOAT3(25, 20, 28));
			rocks[i]->SetRotate(XMFLOAT3(0, 60, 0));
		}
	}

	rocks[0]->SetPos(XMFLOAT3(330, 0, -250));
	rocks[1]->SetPos(XMFLOAT3(-310, 0, 230));
	rocks[2]->SetPos(XMFLOAT3(-230, 0, 330));
	rocks[3]->SetPos(XMFLOAT3(230, 0, -380));
	rocks[4]->SetPos(XMFLOAT3(330, 0, 250));
	rocks[5]->SetPos(XMFLOAT3(-310, 0, -230));
	rocks[6]->SetPos(XMFLOAT3(-230, 0, -330));
	rocks[7]->SetPos(XMFLOAT3(230, 0, 380));

	rocks[8]->SetPos(XMFLOAT3(330, 0, 70));
	rocks[9]->SetPos(XMFLOAT3(310, 0, -80));
	rocks[10]->SetPos(XMFLOAT3(-330, 0, 90));
	rocks[11]->SetPos(XMFLOAT3(-350, 0, -70));
	rocks[12]->SetPos(XMFLOAT3(80, 0, 320));
	rocks[13]->SetPos(XMFLOAT3(-90, 0, 370));
	rocks[14]->SetPos(XMFLOAT3(90, 0, -310));
	rocks[15]->SetPos(XMFLOAT3(-70, 0, -340));

	rocks[16]->SetPos(XMFLOAT3(370, 0, -350));
	rocks[17]->SetPos(XMFLOAT3(-340, 0, 330));
	rocks[18]->SetPos(XMFLOAT3(370, 0, 350));
	rocks[19]->SetPos(XMFLOAT3(-340, 0, -330));
	rocks[20]->SetPos(XMFLOAT3(0, 0, 300));
	rocks[21]->SetPos(XMFLOAT3(0, 0, -300));
	rocks[22]->SetPos(XMFLOAT3(300, 0, 0));
	rocks[23]->SetPos(XMFLOAT3(-300, 0, 0));
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
	for (int i = 0; i <= rockcount - 1; i++)
	{
		rocks[i]->Update();
	}
}

void Stage2D::Draw()
{
	floor->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
	floor2->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
	for (int i = 0; i <= rockcount - 1; i++)
	{
		rocks[i]->Draw(DirectXDevice::cmdList);
	}
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

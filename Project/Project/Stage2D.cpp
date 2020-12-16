#include "Stage2D.h"
#include"DirectXDevice.h"
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
}
void Stage2D::CreateStage(int count)
{

}
void Stage2D::Update()
{
	floor->Update();
	input->Update();
	CameraMove();
	
}

void Stage2D::Draw()
{
	floor->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
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

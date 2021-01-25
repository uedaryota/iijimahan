#include "End.h"
#include"Input.h"
#include"DirectXDevice.h"

//Input* input1;
Sprite* sprite1;

End::End()
{

}

void End::Draw()
{
	Sprite::PreDraw(DirectXDevice::cmdList);
	sprite1->Draw();
	Sprite::PostDraw();
}

void End::Update()
{
	Camera::ReturnCameraState()->eyeangleX = 0;
	Camera::ReturnCameraState()->eyeangleY = 0;
	Camera::ReturnCameraState()->eyeangleZ = 0;

	Camera::ReturnCameraState()->pPos = XMFLOAT3(0, 0, 0);
	Camera::ReturnCameraState()->target= XMFLOAT3(0, 0, 0);
	//input1->Update();
	//if (input1->PushKey(DIK_B))
	{
		endFlag = true;
	}
}

void End::Initialize()
{
	endFlag = false;
	//input1 = new Input();
	//input1->Initialize();
	sprite1->Create(1, { 1, 1 });
	sprite1->SetPosition(XMFLOAT2{ 1 * (static_cast<float>(Camera::window_width) / static_cast<float>(Camera::window_height)),1 });
	//sprite1->SetTex(L"img/end.png");
}

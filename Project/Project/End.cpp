#include "End.h"
#include"Input.h"
#include"Sprite.h"

//Input* input1;
Sprite* sprite1;

End::End()
{

}

void End::Draw()
{
	sprite1->Draw();
}

void End::Update()
{
	Camera::ReturnCameraState()->eyeangleX = 0;
	Camera::ReturnCameraState()->eyeangleY = 0;
	Camera::ReturnCameraState()->eyeangleZ = 0;

	Camera::ReturnCameraState()->pPos = XMFLOAT3(0, 0, 0);
	Camera::ReturnCameraState()->target= XMFLOAT3(0, 0, 0);
	//input1->Update();
	sprite1->Update();
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
	sprite1 = new Sprite();
	sprite1->Initialize();
	//sprite1->SetTex(L"img/end.png");
}

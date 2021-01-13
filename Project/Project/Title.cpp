#include"Title.h"
#include"Input.h"
#include"DirectXDevice.h"

Input* input2;
 void Title::Update()
{

	 Camera::Update();
	 input2->Update();
	 Camera::window_height;
	 sprite->Update();
	 sprite2->Update();

	 obj->Update();
	 if (input2->TriggerKey(DIK_SPACE))
	 {
		 endFlag = true;
	 }
	 if (input2->PushKey(DIK_S))
	 {
		 Camera::ReturnCameraState()->eyeangleY += 0.03;
	 }
}
 Title::Title()
 {
 }
 void Title::Draw()
{
	obj->Draw(DirectXDevice::cmdList);
	sprite->Draw();
	//sprite2->Draw();
 }
void Title::Initialize()
{

	Camera::ReturnCameraState()->cameraPos = { 0,0,500 };
	Camera::ReturnCameraState()->pPos = { 0,0,0 };
	Camera::ReturnCameraState()->target = { 0,0,1 };

	endFlag = false;
	input2 = new Input();
	input2->Initialize();
	sprite = new Sprite();
	sprite->Initialize();
	sprite->scale = { 1 * (static_cast<float>(Camera::window_width) / static_cast<float>(Camera::window_height)),1,1 };
	sprite->ResetTex(L"img/TowerDefence_Title.png");
	sprite2 = new Sprite();
	sprite2->Initialize();
	sprite2->scale = { 1 * (static_cast<float>(Camera::window_width) / static_cast<float>(Camera::window_height)),1,1 };
	sprite2->ResetTex(L"img/TowerDefence_TitleBack.png");
	obj = new ObjFile();
	obj->Initialize();
	obj->LoadObj("BackSphere");
	obj->scale={1000, 1000, 1000};
}
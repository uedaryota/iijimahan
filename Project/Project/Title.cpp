#include"Title.h"
#include"Input.h"
#include"DirectXDevice.h"

Input* input2;
 void Title::Update()
{

	 Camera::Update();
	 input2->Update();
	 Camera::window_height;

	 obj->Update();
	 if (input2->TriggerKey(DIK_SPACE))
	 {
		 endFlag = true;
	 }
	Camera::ReturnCameraState()->eyeangleY += 0.01;
	sound->ChkRoop();
}
 Title::Title()
 {
 }
 void Title::Draw()
{
	obj->Draw(DirectXDevice::cmdList);
	Sprite::PreDraw(DirectXDevice::cmdList);
	sprite->Draw();
	//sprite2->Draw();
	Sprite::PostDraw();
 }
void Title::Initialize()
{
	Camera::ReturnCameraState()->cameraPos = { 0,0,500 };
	Camera::ReturnCameraState()->pPos = { 0,0,0 };
	Camera::ReturnCameraState()->target = { 0,0,1 };

	endFlag = false;
	input2 = new Input();
	input2->Initialize();
	Sprite::LoadTexture(0, L"img/debugfont.png");
	Sprite::LoadTexture(1, L"img/TowerDefence_Title.png");
	Sprite::LoadTexture(2, L"img/TowerDefence_TitleBack.png");

	sprite = Sprite::Create(1, {0.0f, 0.0f});
	//sprite->SetPosition(XMFLOAT2{ static_cast<float>(Camera::window_width) / 2, static_cast<float>(Camera::window_height) / 2 });
	sprite->SetSize(XMFLOAT2{ static_cast<float>(Camera::window_width), static_cast<float>(Camera::window_height) });
	sprite2 = Sprite::Create(2, { 0.0f, 0.0f });
	sprite2->SetPosition(XMFLOAT2{static_cast<float>(Camera::window_width) / 2, static_cast<float>(Camera::window_height) / 2});
	sprite2->SetSize(XMFLOAT2{ 1 * (static_cast<float>(Camera::window_width) / static_cast<float>(Camera::window_height)),1 });
	obj = new ObjFile();
	obj->Initialize();
	obj->LoadObj("BackSphere");
	obj->scale={1000, 1000, 1000};
	sound->Initialize();
	sound->LoadFile(L".\\Resources\\TDBGM2.mp3");
	sound->PlayRoop();
}
#include"Title.h"
#include"Input.h"
#include"Sprite.h"
Input* input2;
Sprite* sprite; 

 void Title::Update()
{
	 input2->Update();
	 sprite->Update();
	// if (input->TriggerKey(DIK_SPACE))
	 {
		 endFlag = true;
	 }
}
 Title::Title()
 {
 }
 void Title::Draw()
{
	sprite->Draw();
}
void Title::Initialize()
{
	endFlag = false;
	input2 = new Input();
	input2->Initialize();
	sprite = new Sprite();
	sprite->Initialize();
	sprite->ResetTex(L"img/TowerDefence_Title.png");
}
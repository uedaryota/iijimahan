#include "Tower.h"
#include"Input.h"
Tower::Tower()
{

}

Tower::~Tower()
{
	delete(obj);
}

void Tower::Initialize(ID3D12Device* dev)
{
	Sprite::LoadTexture(4, L"Resources/HPBack.png");
	Sprite::LoadTexture(5, L"Resources/HP.png");
	sprite = Sprite::Create(4, XMFLOAT2(Camera::window_width / 2, 25));
	sprite->SetAnchorPoint(XMFLOAT2(0.5, 0));
	sprite->SetSize(XMFLOAT2(400, 50));

	maxhp = hp;
	nowhp = hp;

	sprite2 = Sprite::Create(5, XMFLOAT2(Camera::window_width / 2 - 200, 0));
	sprite2->SetSize(XMFLOAT2(spriteSize.x / maxhp * nowhp, spriteSize.y));

	text = Text::GetInstance();
	text->Initialize(0);

	obj->Initialize();
	obj->LoadObj("Tower");
	obj->position.y = 10;
	obj->SetScale({ 30,30,30 });
}

void Tower::Update()
{
	if (hp <= 0)
	{
		hp = 0;
	}
	if (nowhp > hp)
	{
		nowhp -= hpspeed;
	}
	if (nowhp < hp)
	{
		nowhp = hp;
	}
	sprite2->SetSize(XMFLOAT2(spriteSize.x / maxhp * nowhp, spriteSize.y));
	obj->Update();
}

void Tower::Draw(ID3D12GraphicsCommandList * cmdList)
{
	obj->Draw(cmdList);
	Sprite::PreDraw(DirectXDevice::cmdList);
	sprite->Draw();
	sprite2->Draw();
	if (hp < 10)
	{
		text->Print("HP   " + to_string((int)hp) + " / " + to_string((int)maxhp), Camera::window_width / 2 - 100, 8, 2);
	}
	if (hp < 100 && hp >= 10)
	{
		text->Print("HP  " + to_string((int)hp) + " / " + to_string((int)maxhp), Camera::window_width / 2 - 100, 8, 2);
	}
	if (hp >= 100)
	{
		text->Print("HP " + to_string((int)hp) + " / " + to_string((int)maxhp), Camera::window_width / 2 - 100, 8, 2);
	}
	Sprite::PostDraw();
}


XMFLOAT3 Tower::GetPosition()
{
	XMFLOAT3 position;
	position = obj->position;
	return position;
}

void Tower::SetPoisition(XMFLOAT3 position)
{
	obj->position = position;
}

float Tower::GetHp()
{
	return hp;
}

void Tower::SetHp(float x)
{
	hp = x;
}

void Tower::Damage(float damage)
{
	hp = hp - damage;
}

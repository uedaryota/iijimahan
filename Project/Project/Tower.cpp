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
	Sprite::LoadTexture(6, L"img/HPBack.png");
	Sprite::LoadTexture(7, L"img/HP.png");
	hp = 100;
	sprite = Sprite::Create(6, XMFLOAT2(Camera::window_width / 2, 25));
	sprite->SetAnchorPoint(XMFLOAT2(0.5, 0));
	sprite->SetSize(XMFLOAT2(400, 50));

	maxhp = hp;
	nowhp = hp;

	sprite2 = Sprite::Create(7, XMFLOAT2(Camera::window_width / 2 - 200, 0));
	sprite2->SetSize(XMFLOAT2(spriteSize.x / maxhp * nowhp, spriteSize.y));

	text = Text::GetInstance();
	text->Initialize(0);

	obj = new ObjFile();
	obj->Initialize();
	obj->LoadObj("Tower");
	obj->position.y = 10;
	obj->SetScale({ 30,30,30 });
	if (particle == nullptr)
	{
		particle = new ParticleManager();
	}
	particle->Initialize();
	sound->Initialize();
	sound->LoadFile(L".\\Resources\\explode.mp3");
}

void Tower::Initialize2()
{
	Sprite::LoadTexture(6, L"img/HPBack.png");
	Sprite::LoadTexture(7, L"img/HP.png");
	hp = 100;
	sprite = Sprite::Create(6, XMFLOAT2(Camera::window_width / 2, 25));
	sprite->SetAnchorPoint(XMFLOAT2(0.5, 0));
	sprite->SetSize(XMFLOAT2(400, 50));

	maxhp = hp;
	nowhp = hp;

	sprite2 = Sprite::Create(7, XMFLOAT2(Camera::window_width / 2 - 200, 0));
	sprite2->SetSize(XMFLOAT2(spriteSize.x / maxhp * nowhp, spriteSize.y));

	text = Text::GetInstance();
	text->Initialize(0);
	obj = new ObjFile();
	obj->Initialize();
	obj->LoadObj("Tower");
	obj->position.y = 10;
	obj->SetScale({ 30,30,30 });
	//particle = new ParticleManager();
	particle->Initialize();
	sound->Initialize();
	sound->LoadFile(L".\\Resources\\explode.mp3");
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
	//particle->Start();
	particle->SetPos(obj->position);
	particle->Update();
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
	particle->Draw();
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
	particle->Start();
	int movetime = 25;
	Camera::CameraShake(movetime);
	sound->PlayStart();
}

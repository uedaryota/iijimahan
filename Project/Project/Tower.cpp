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
	texPos = { 0,Camera::window_height - 100,0 };

	tex->Initialize();
	tex->position = { texPos.x, texPos.y / Camera::window_height, 0 };
	tex->scale = { texSize.x / Camera::window_width*hp,texSize.y / Camera::window_height,100.0f };
	obj->Initialize(dev);
	obj->LoadObj("triangle_mat");
	obj->position.y = 10;
	hp = 100;
}

void Tower::Update()
{
	if (hp <= 0)
	{
		hp = 0;
	}
	tex->position = { (texPos.x - ((100 - hp)*texSize.x / 2)) / Camera::window_width,
		texPos.y / Camera::window_height, 0 };//プロジェクションView行列掛けてないのでここで
	tex->scale = { texSize.x / Camera::window_width*hp,texSize.y / Camera::window_height,100.0f };//同文
	obj->Update();
	tex->Update();
}

void Tower::Draw(ID3D12GraphicsCommandList * cmdList)
{
	obj->Draw(cmdList);
	tex->Draw();
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

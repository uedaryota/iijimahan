#include "Collision.h"
#include"DirectXDevice.h"
Collision::Collision()
{
}

Collision::~Collision()
{
}

void Collision::Update()
{
}

void Collision::Draw()
{
}

void Collision::Initialize()
{
}

bool Collision::MouseToCircle(Input input, CircleCollision circle)
{
	XMFLOAT3 pos;
	pos = input.GetMousePos();
	//座標をカメラ基準に変換
	pos.x = (pos.x - Camera::window_width / 2) / (Camera::window_width / 2);
	pos.y = (-pos.y + Camera::window_height / 2) / (Camera::window_height / 2);

	////
	//pos = { pos.x + Camera::MainCameraPos().x,
	//	pos.y + Camera::MainCameraPos().y,
	//	pos.z + Camera::MainCameraPos().z };

	////ターゲットとカメラの外積
	//XMFLOAT3 v = { Camera::ReturnCameraState()->target.y*Camera::MainCameraPos().z - Camera::ReturnCameraState()->target.z*Camera::MainCameraPos().y ,
	// Camera::ReturnCameraState()->target.z*Camera::MainCameraPos().x - Camera::ReturnCameraState()->target.x*Camera::MainCameraPos().z ,
	// Camera::ReturnCameraState()->target.x*Camera::MainCameraPos().y - Camera::ReturnCameraState()->target.y*Camera::MainCameraPos().x , };
	//
	//float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
	////正規化
	//v = { v.x / len, v.y / len, v.z / len };
	//

	return false;
}

bool Collision::CircleToCircle(CircleCollision a, CircleCollision b)
{
	XMFLOAT3 v = { fabs(a.position.x - b.position.x), fabs(a.position.y - b.position.y), fabs(a.position.z - b.position.z) };
	float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
	if (len < a.length + b.length)
	{
		return true;
	}
	return false;
}

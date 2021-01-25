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

bool Collision::MouseToCircle(Input* input, CircleCollision* circle)
{
	XMFLOAT3 pos;
	pos = input->GetMousePos();
	
	pos.z = 1;
	XMMATRIX invView;
	XMMATRIX invPri;
	XMMATRIX invPort;
	invPort = XMMatrixIdentity();
	invPort.r[0].m128_f32[0] = Camera::window_width / 2;
	invPort.r[3].m128_f32[0] = Camera::window_width / 2;
	invPort.r[1].m128_f32[1] = -Camera::window_height / 2;
	invPort.r[3].m128_f32[1] = Camera::window_height / 2;
	invPort = XMMatrixInverse(nullptr, invPort);

	invView = XMMatrixInverse(nullptr, Camera::ReturnCameraState()->matView);
	invPri = XMMatrixInverse(nullptr, Camera::ReturnCameraState()->matProjection);
	//À•W‚ðƒJƒƒ‰Šî€‚É•ÏŠ·
	/*pos.x = (pos.x - Camera::window_width / 2) / (Camera::window_width / 2);
	pos.y = (-pos.y + Camera::window_height / 2) / (Camera::window_height / 2);
*/
	XMMATRIX tmp = invPort * invPri*invView;
	XMVECTOR vecpos = { pos.x,pos.y,pos.z };
	XMVECTOR vec = XMVector3TransformCoord(vecpos, tmp);
	vec = XMVector3Normalize(vec);
	XMFLOAT3 dir = { vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2] };
	XMFLOAT3 start = Camera::MainCameraPos();
	//start = { start.x + dir.x * 300 , start.y + dir.y * 300 , start.z + dir.z * 300};
	XMFLOAT3 m{ start.x - circle->position.x ,start.y - circle->position.y, start.z - circle->position.z };
	//float len = sqrtf(m.x*m.x + m.y*m.y + m.z*m.z);
	//m = { m.x / len, m.y / len, m.z / len };
	float b, c;
	b = m.x*dir.x + m.y*dir.y + m.z*dir.z;
	c =(m.x*m.x + m.y*m.y + m.z*m.z) - (circle->scale*circle->scale);

	if (c > 0 && b > 0)
	{
		return false;
	}
	float dis = b * b - c;
	if (dis < 0)
	{
		return false;
	}
	return true;
//	return false;
}

bool Collision::CircleToCircle(CircleCollision* a,CircleCollision* b)
{
	XMFLOAT3 v = { fabs(a->position.x - b->position.x), fabs(a->position.y - b->position.y), fabs(a->position.z - b->position.z) };
	float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
	if (len < a->scale + b->scale)
	{
		return true;
	}
	return false;
}

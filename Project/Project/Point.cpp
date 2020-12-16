#include "Point.h"
#include"DirectXDevice.h"
Point::Point()
{
}

Point::~Point()
{
	delete(pol);
}

void Point::Initialize()
{
	pol->Initialize();
}

void Point::Update()
{
	pol->Update();
	//vel = //ai.UpdateAI();
}

void Point::SetPos(XMFLOAT3 position)
{
	pol->position = position;
}

void Point::SetScale(XMFLOAT3 scale)
{
	pol->scale = scale;
}

XMFLOAT3 Point::GetAncer()
{
	return pol->position;
}

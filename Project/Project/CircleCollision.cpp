#include "CircleCollision.h"

CircleCollision::CircleCollision()
{
}

CircleCollision::~CircleCollision()
{
}

bool CircleCollision::CollisionUpdate(CircleCollision other)
{
	if (fabsf(position.x - other.position.x) < length + other.length&&
		fabsf(position.y - other.position.y) < length + other.length&&
		fabsf(position.z - other.position.z) < length + other.length)
	{
		return true;
	}
	return false;
}

void CircleCollision::Draw()
{
}

void CircleCollision::SetPos(XMFLOAT3 pos)
{
}

void CircleCollision::SetLength(float len)
{
}

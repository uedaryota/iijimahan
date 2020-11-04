#include "CircleCollision.h"

CircleCollision::CircleCollision()
{
}

CircleCollision::~CircleCollision()
{
}

bool CircleCollision::CollisionUpdate(CircleCollision other)
{
	if (abs(position.x - other.position.x) < length + other.length&&
		abs(position.y - other.position.y) < length + other.length&&
		abs(position.z - other.position.z) < length + other.length)
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

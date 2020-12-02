#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class CircleCollision
{
public:
	CircleCollision();
	~CircleCollision();
	bool CollisionUpdate(CircleCollision other);//�����蔻��
	void Draw();//�`��
	void SetPos(XMFLOAT3 pos);//���W�ύX
	void SetLength(float len);//���a�ύX
private:
	float length = 0.5f;//���a
	XMFLOAT3 position;//���W
};

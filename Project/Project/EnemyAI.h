#pragma once
#include"Poly.h"
#include"Tower.h"
#include"Point.h"


class EnemyAI//AI�N���X�̊�Ղł��BAI�𑝂₷�ꍇ�������p�������č쐬���܂��B
{
public:
	EnemyAI();
	//~EnemyAI();
	void Initialize();	
	void SetPos(XMFLOAT3 position);
	void SetTarget(Tower tower);

	float GetPower();

	void SetPower(float x);
	XMFLOAT3 position;
	XMFLOAT3 velocity;
	EnemyAI GetAI(EnemyAI ai);
	Tower tower,pointA,pointB;

	float Power=1;
private:
	Poly* pol = new Poly();
	XMFLOAT3 vel = { -1, 0, 1 };
	bool NextX = false;
	bool NextZ = false;
	float Floar2 = 110.0f;
};

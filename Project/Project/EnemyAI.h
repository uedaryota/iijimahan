#pragma once
#include"Poly.h"
#include"Tower.h"
class EnemyAI//AI�N���X�̊�Ղł��BAI�𑝂₷�ꍇ�������p�������č쐬���܂��B
{
public:
	EnemyAI();
	//~EnemyAI();
	void Initialize();
	XMFLOAT3 UpdateAI();
	void SetPos(XMFLOAT3 position);
	void SetTarget(Tower tower);
	float GetHp();
	float GetSpeed();
	float GetPower();
	XMFLOAT3 position;
	XMFLOAT3 velocity;
	EnemyAI GetAI(EnemyAI ai);
	Tower tower;
	float Hp;
	float Speed;
	float Power;
private:
	Poly* pol = new Poly();
};


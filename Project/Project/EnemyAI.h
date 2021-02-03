#pragma once
#include"Poly.h"
#include"Tower.h"
#include"Point.h"


class EnemyAI//AIクラスの基盤です。AIを増やす場合こいつを継承させて作成します。
{
public:
	EnemyAI();
	//~EnemyAI();
	void Initialize();	
	void SetPos(XMFLOAT3 position);
	void SetTarget(Tower tower);


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

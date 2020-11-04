#pragma once
#include"Poly.h"
#include"Tower.h"
class EnemyAI //AIクラスの基盤です。AIを増やす場合こいつを継承させて作成します。
{
public:
	EnemyAI();
	~EnemyAI();
	void Initialize();
	void Update();
	void SetPos(XMFLOAT3 position);
	void SetTarget(Tower tower);
	float GetHp();
	float GetSpeed();
	float GetPower();
	XMFLOAT3 position;
	EnemyAI GetAI(EnemyAI ai);
	Tower tower;
	float Hp;
	float Speed;
	float Power;
private:
	Poly* pol = new Poly();
};


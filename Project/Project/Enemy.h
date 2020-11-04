#pragma once
#include"Poly.h"
#include"EnemyAI.h"

class Enemy
{
public:
	Enemy();
	~Enemy();
	void Initialize();
	void Update();
	void Draw();
	void SetPos(XMFLOAT3 position);
	void SetScale(XMFLOAT3 scale);
	void Install(EnemyAI Ai);
	void SetState();
	EnemyAI ai;
	float Hp;
	float Speed;
	float Power;
private:
	Poly * pol = new Poly();
};
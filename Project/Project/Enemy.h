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
	void PositionUpdate();
	EnemyAI ai;
	float Hp;
	float Speed;
	float Power;
	XMFLOAT3 vel = {-1, 0, -1};
private:
	Poly * pol = new Poly();
};
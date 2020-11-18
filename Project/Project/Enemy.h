#pragma once
#include"Poly.h"
#include"EnemyAI.h"

enum State {
	move1, move2, move3, atack1, atack2, Destory
};

class Enemy :EnemyAI
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
	void PositionUpdate(XMFLOAT3 pointA, XMFLOAT3 pointB, XMFLOAT3 tower);
	EnemyAI ai;
	float Hp;
	float Speed;
	float Power;
	XMFLOAT3 vel = {-1, 0, -1};
	bool NextX = false;
	bool NextZ = false;
	float Floar2 = 110.0f;
	State state;
	State GetState();
private:
	Poly * pol = new Poly();
};
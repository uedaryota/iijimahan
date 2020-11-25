#pragma once
#include"Poly.h"
#include"EnemyAI.h"

enum State {
	move1, move2, move3, atack1, atack2, Destory
};

enum CodeOfConduct {
	Lazy/*�ʓ|�L����*/,Pride/*���ʂڂ�*/,Coward/*�����ʂ�*/,Sage/*�m�b��*/,
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
	XMFLOAT3 vel = {-0.1, 0, -0.1};
	bool NextX = false;
	bool NextZ = false;
	float Floar2 = 110.0f;
	State state;
	State GetState();
	void Avoid();//������s���ł��B
	bool AvoidAct = false;//�ڐG���肪�������ꍇ�͂��̃t���O��TRUE�ɕύX���āA��Q�����痣���s���������܂��B
	float Cnt = 0;
	void SetTarget(XMFLOAT3* x);
	XMFLOAT3 TargetTower{ 15,15,15 };
	void SetTower(Tower* tow);
	Tower* mokuhyou;
	void TowerAtack();
private:
	Poly * pol = new Poly();
};
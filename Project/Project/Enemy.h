#pragma once
#include"Poly.h"
#include"EnemyAI.h"
#include <d3dx12.h>
#include"Texture.h"
#include"CircleCollision.h"

using namespace DirectX;
enum State {
	move1, move2, move3, atack1, special1, Destory, Stay, 
};

enum CodeOfConduct {
	Lazy/*�ʓ|�L����*/,Pride/*���ʂڂ�*/,Coward/*�����ʂ�*/,Sage/*�m�b��*/,
};
class Enemy :EnemyAI
{
public:
	Enemy();
	~Enemy();
	Enemy(const Enemy *ene);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList * cmdList);
	void SetPos(XMFLOAT3 position);
	void SetScale(XMFLOAT3 scale);
	void Install(EnemyAI Ai);
	void SetState();
	void PositionUpdate(XMFLOAT3 pointA, XMFLOAT3 pointB, XMFLOAT3 tower);
	EnemyAI ai;
	float Hp;
	float Speed=1;
	float Power;
	int time = 0;
	int endtime = 3;
	XMFLOAT3 endscale = {10, 10, 10};
	XMFLOAT3 vel = {0.3, 0, -0.3};
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
	void GetAlive();//�����Ă邩�H
	bool Deadflag = false;
	bool Endflag = false;
	bool Dead = false;
	CodeOfConduct code;
	CodeOfConduct GetCode();
	void EnemyDamege(float x);//�G�l�~�[�ɑ΂���U�������ł��Bfloat�Ƀ_���[�W�ʂ�����B
	XMFLOAT3 SetAncer1(XMFLOAT3 x);
	XMFLOAT3 SetAncer2(XMFLOAT3 x);
	XMFLOAT3 Ancer1{ 15,15,15 };
	XMFLOAT3 Ancer2{ 10,10,10 };
	void ActionRiset();
	XMFLOAT3 GetPosition();

	void SpeedEne();
	float GetHp();
	float GetSpeed();
	void SetHp(float x);
	void SetSpeed(float x);
	float GetPower();
	void SetPower(float x);
	CircleCollision* col;
	
private:
	//���i���ɍs���ω����邽�߂̕ϐ��ł�
	bool Damege = false;//�_���[�W���󂯂��ۂ�TRUE�ɂȂ�ϐ��ł�
	bool Kosinuke = false;//��������Ԃł��B
	ObjFile* obj = new ObjFile();
	ObjFile* obj2 = new ObjFile();
	Texture* tex = new Texture();
	
};
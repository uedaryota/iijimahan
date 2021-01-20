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
	Lazy/*面倒臭がり*/,Pride/*うぬぼれ*/,Coward/*こしぬけ*/,Sage/*知恵者*/,
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
	XMFLOAT3 vel = {0.1 + (Speed / 50), 0, -0.1 - (Speed / 50) };
	bool NextX = false;
	bool NextZ = false;
	float Floar2 = 110.0f;
	State state;
	State GetState();
	void Avoid();//避ける行動です。
	bool AvoidAct = false;//接触判定があった場合はこのフラグをTRUEに変更して、障害物から離れる行動をさせます。
	float Cnt = 0;
	void SetTarget(XMFLOAT3* x);
	XMFLOAT3 TargetTower{ 15,15,15 };
	void SetTower(Tower* tow);
	Tower* mokuhyou;
	void TowerAtack();
	void GetAlive();//生きてるか？
	bool Deadflag = false;
	CodeOfConduct code;
	CodeOfConduct GetCode();
	void EnemyDamege(float x);//エネミーに対する攻撃処理です。floatにダメージ量を入れる。
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
	CircleCollision* col;
private:
	Poly * pol = new Poly();

	//性格事に行動変化するための変数です
	bool Damege = false;//ダメージを受けた際にTRUEになる変数です
	bool Kosinuke = false;//腰抜け状態です。
	ObjFile* obj = new ObjFile();
	Texture* tex = new Texture();
	
};
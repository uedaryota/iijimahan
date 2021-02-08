#pragma once
#include"Battery.h"
#include"CircleCollision.h"
#include"DefenderWindow.h"
class DefenderSpawn
{
public:
	DefenderSpawn();
	~DefenderSpawn();
	void Update();
	void Draw();
	void Initialize();
	void CreateBattery();
	void StopCreate();
	void SetPos(XMFLOAT3 pos);
	bool ReturnCostFlag();
	void CostFlagTrue();
	void CostFlagFalse();
	int ReturnUseCost();
	ObjFile* obj = new ObjFile();
	ObjFile* obj2 = new ObjFile();

	Battery* battery = new Battery();
	CircleCollision* col = new CircleCollision();
	

	XMFLOAT3 position;
	bool costFlag = false;
	bool possibleSet = false;
	const int usecost = 20;
private:
};


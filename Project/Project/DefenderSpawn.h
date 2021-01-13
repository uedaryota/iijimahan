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
	void SetPos(XMFLOAT3 pos);
	Battery* battery = nullptr;
	CircleCollision* col = nullptr;
	XMFLOAT3 position;
};


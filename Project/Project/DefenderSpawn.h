#pragma once
#include"Battery.h"
#include"CircleCollision.h"
#include"DefenderWindow.h"
class DefenderSpawn
{
	DefenderSpawn();
	~DefenderSpawn();
	void Update();
	void Draw();
	void Initialize();
	void CreateBattery();
	Battery* battery;
};


#pragma once
#include"Battery.h"
class DefenderSpawn
{
	DefenderSpawn();
	~DefenderSpawn();
	void Update();
	void Draw();
	void Initialize();

	Battery* battery;
};


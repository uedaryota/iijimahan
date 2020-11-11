#pragma once
#include"Floor.h"
class Stage
{
public:
	Stage();
	void Initialize();
	void Update();
	void Draw();

private:
	Floor* floor1_1 = new Floor();
	Floor* floor1_2 = new Floor();
	Floor* floor1_3 = new Floor();

	Floor* floor2_1 = new Floor();
	Floor* floor2_2 = new Floor();
	Floor* floor2_3 = new Floor();

	Floor* floor3_1 = new Floor();
	Floor* floor3_2 = new Floor();
	Floor* floor3_3 = new Floor();

	int floorCount = 1;
	float speed = 0.3;
	bool push = false;
};


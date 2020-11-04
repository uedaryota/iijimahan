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
	Floor* floor1 = new Floor();
	Floor* floor2 = new Floor();
	Floor* floor3 = new Floor();
};


#pragma once
#include"Floor.h"
#include"Input.h"
class Stage
{
public:
	Stage();
	void Initialize();
	void CreateStage(int count);
	void Update();
	void Draw();

private:
	
	std::vector<Floor*>stageList;
	int floorCount = 1;
	int allFloor = 5;
	float speed = 0.3;
	bool push = false;
	Input* input;
};


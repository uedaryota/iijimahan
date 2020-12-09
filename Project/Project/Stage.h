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
	void CameraMove();
	std::vector<Floor*>stageList;
	//現在フロア
	int floorCount = 1;
	//全体フロア
	int allFloor = 5;
	float speed = 0.3;
	bool push = false;
	Input* input;
};


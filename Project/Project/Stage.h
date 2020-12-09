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
	//���݃t���A
	int floorCount = 1;
	//�S�̃t���A
	int allFloor = 5;
	float speed = 0.3;
	bool push = false;
	Input* input;
};


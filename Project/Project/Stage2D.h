#pragma once
#include"Floor.h"
#include"Input.h"
#include"ObjFile.h"
class Stage2D
{
public:
	Stage2D();
	~Stage2D();
	void Initialize();
	void CreateStage(int count);
	void Update();
	void Draw();

private:
	void CameraMove();
	Input* input = new Input();
	//ステージ本体
	Floor* floor = new Floor();
	//背景に続く床
	Floor* floor2 = new Floor();

	int rockcount = 24;

	ObjFile* rocks[24];
	bool initializeFlag = false;
};


#pragma once
#include"Floor.h"
#include"Input.h"
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
	Input* input;
	//ステージ本体
	Floor* floor;
	//背景に続く床
	Floor* floor2;

};


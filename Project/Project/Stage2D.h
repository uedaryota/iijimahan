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
	Input* input;
	//ステージ本体
	Floor* floor;
	//背景に続く床
	Floor* floor2;
	//周りの岩1
	ObjFile* rock = new ObjFile();
	//周りの岩2
	ObjFile* rock2 = new ObjFile();
	//回りの岩3
	ObjFile* rock3 = new ObjFile();
	//回りの岩4
	ObjFile* rock4 = new ObjFile();
	//回りの岩5
	ObjFile* rock5 = new ObjFile();
	//回りの岩6
	ObjFile* rock6 = new ObjFile();
};


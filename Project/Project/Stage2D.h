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
	//回りの岩7
	ObjFile* rock7 = new ObjFile();
	//回りの岩8
	ObjFile* rock8 = new ObjFile();
	//回りの岩9
	ObjFile* rock9 = new ObjFile();
	//回りの岩10
	ObjFile* rock10 = new ObjFile();
	//回りの岩11
	ObjFile* rock11 = new ObjFile();
	//回りの岩12
	ObjFile* rock12 = new ObjFile();
	//周りの岩13
	ObjFile* rock13 = new ObjFile();
	//周りの岩14
	ObjFile* rock14 = new ObjFile();
	//回りの岩15
	ObjFile* rock15 = new ObjFile();
	//回りの岩16
	ObjFile* rock16 = new ObjFile();
	//回りの岩17
	ObjFile* rock17 = new ObjFile();
	//回りの岩18
	ObjFile* rock18 = new ObjFile();
	//回りの岩19
	ObjFile* rock19 = new ObjFile();
	//回りの岩20
	ObjFile* rock20 = new ObjFile();
	//回りの岩21
	ObjFile* rock21 = new ObjFile();
	//回りの岩22
	ObjFile* rock22 = new ObjFile();
	//回りの岩23
	ObjFile* rock23 = new ObjFile();
	//回りの岩24
	ObjFile* rock24 = new ObjFile();

	int rockcount = 24;

	ObjFile* rocks[24];
};


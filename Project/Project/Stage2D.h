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
	//�X�e�[�W�{��
	Floor* floor;
	//�w�i�ɑ�����
	Floor* floor2;
	//����̊�1
	ObjFile* rock = new ObjFile();
	//����̊�2
	ObjFile* rock2 = new ObjFile();
	//���̊�3
	ObjFile* rock3 = new ObjFile();
	//���̊�4
	ObjFile* rock4 = new ObjFile();
	//���̊�5
	ObjFile* rock5 = new ObjFile();
	//���̊�6
	ObjFile* rock6 = new ObjFile();
	//���̊�7
	ObjFile* rock7 = new ObjFile();
	//���̊�8
	ObjFile* rock8 = new ObjFile();
	//���̊�9
	ObjFile* rock9 = new ObjFile();
	//���̊�10
	ObjFile* rock10 = new ObjFile();
	//���̊�11
	ObjFile* rock11 = new ObjFile();
	//���̊�12
	ObjFile* rock12 = new ObjFile();
	//����̊�13
	ObjFile* rock13 = new ObjFile();
	//����̊�14
	ObjFile* rock14 = new ObjFile();
	//���̊�15
	ObjFile* rock15 = new ObjFile();
	//���̊�16
	ObjFile* rock16 = new ObjFile();
	//���̊�17
	ObjFile* rock17 = new ObjFile();
	//���̊�18
	ObjFile* rock18 = new ObjFile();
	//���̊�19
	ObjFile* rock19 = new ObjFile();
	//���̊�20
	ObjFile* rock20 = new ObjFile();
	//���̊�21
	ObjFile* rock21 = new ObjFile();
	//���̊�22
	ObjFile* rock22 = new ObjFile();
	//���̊�23
	ObjFile* rock23 = new ObjFile();
	//���̊�24
	ObjFile* rock24 = new ObjFile();

	int rockcount = 24;

	ObjFile* rocks[24];
};


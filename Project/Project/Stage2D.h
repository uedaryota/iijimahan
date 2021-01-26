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
};


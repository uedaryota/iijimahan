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
	//�X�e�[�W�{��
	Floor* floor;
	//�w�i�ɑ�����
	Floor* floor2;

};


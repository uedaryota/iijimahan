#pragma once
#include"ObjFile.h"
#include"Bullet.h"
#include<vector>
#include"Input.h"
class Battery
{
public:
	Battery();
	~Battery();
	void Update();
	void Draw();
	void Initialize();
	void SetTarget(XMFLOAT3 targetpos);
	void SetPos(XMFLOAT3 pos);
	void SetScale(XMFLOAT3 scale);
	void Shot();
	ObjFile* obj=nullptr;

	std::vector<Bullet*>bulletList;
	XMFLOAT3 mainPos{};
	XMFLOAT3 mainScale{};
	XMFLOAT3 mainRotate{};
	XMFLOAT3 targetPos;
	int interval;
	int damage;
	Input* input;

};


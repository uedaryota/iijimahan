#pragma once
#include"ObjFile.h"
#include"Bullet.h"
#include<vector>
#include"Input.h"
#include"CircleCollision.h"
class Battery
{
public:
	Battery();
	~Battery();
	void Update();
	void Draw();
	void Initialize();
	void SetTarget(XMFLOAT3* Targetpos);
	void SetPos(XMFLOAT3 pos);
	void SetScale(XMFLOAT3 scale);
	void Shot();
	ObjFile* obj=nullptr;
	CircleCollision* col = nullptr;
	CircleCollision* clickcol = nullptr;
	std::vector<Bullet*>bulletList;
	XMFLOAT3 mainPos{};
	XMFLOAT3 mainScale{};
	XMFLOAT3 mainRotate{};
	XMFLOAT3* targetPos = nullptr;
	int interval;
	int damage=1;
	Input* input;

};


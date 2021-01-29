#pragma once
#include"ObjFile.h"
#include"Bullet.h"
#include<vector>
#include"Input.h"
#include"CircleCollision.h"
#include "Sound.h"
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
	void Break();
	ObjFile* obj=nullptr;
	CircleCollision* col = nullptr;
	CircleCollision* clickcol = nullptr;
	std::vector<Bullet*>bulletList;
	XMFLOAT3 position;
	XMFLOAT3 scale{};
	XMFLOAT3 rotation{};
	XMFLOAT3* targetPos = nullptr;
	int interval = 60;
	int damage = 1;
	Input* input;
	bool liveFlag;
	bool MoveFlag;
	
private:
	void Setdir();
	int startval = 60;
	Sound* sound = new Sound();
};


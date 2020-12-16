#pragma once
#include"ObjFile.h"
class Battery
{
public:
	Battery();
	~Battery();
	void Update();
	void Draw();
	void Initialize();
	void SetPos(XMFLOAT3 pos);
	void SetScale(XMFLOAT3 scale);
	ObjFile* obj;

	XMFLOAT3 mainPos{};
	XMFLOAT3 mainScale{};
	XMFLOAT3 mainRotate{};

	int interval;
	int damage;


};


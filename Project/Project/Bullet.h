#pragma once
#include<DirectXMath.h>
#include"ObjFile.h"
using namespace DirectX;
class Bullet
{
public:
	Bullet();
	~Bullet();
	void Update();
	void Draw();
	void Initialize();
	void SetVelocity(XMFLOAT3 velocity);
	void SetPos(XMFLOAT3 pos);
	XMFLOAT3 position{};
	XMFLOAT3 velocity{};
	XMFLOAT3 rotation{};
	XMFLOAT3 scale{};

	ObjFile* obj;
	float speed;

};


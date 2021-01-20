#pragma once
#include<DirectXMath.h>
#include"ObjFile.h"
#include"CircleCollision.h"
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
	void Reset();
	XMFLOAT3 position{};
	XMFLOAT3 velocity{};
	XMFLOAT3 rotation{};
	XMFLOAT3 scale{};
	//�����Ă��邩
	bool liveFlag;
	ObjFile* obj;
	CircleCollision* col;
	
	XMFLOAT3* targetPos = nullptr;
	float speed=3;


};


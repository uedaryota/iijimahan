#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class CircleCollision
{
public:
	CircleCollision();
	~CircleCollision();
	bool CollisionUpdate(CircleCollision other);//当たり判定
	void Draw();//描画
	void SetPos(XMFLOAT3 pos);//座標変更
	void SetLength(float len);//半径変更
private:
	float length = 0.5f;//半径
	XMFLOAT3 position;//座標
};


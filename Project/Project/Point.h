#pragma once
#include"Poly.h"
#include"EnemyAI.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Point
{
public:
	Point();
	~Point();
	void Initialize();
	void Update();
	void SetPos(XMFLOAT3 position);
	void SetScale(XMFLOAT3 scale);
	XMFLOAT3 GetAncer();
private:
	Poly * pol = new Poly();
};
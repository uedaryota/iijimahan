#pragma once
#include"Poly.h"
class Block
{
public:
	Block();
	~Block();
	void Initialize();
	void Update();
	void Draw();
	void SetPos(XMFLOAT3 position);
	void SetScale(XMFLOAT3 scale);

private:
	Poly* pol = new Poly();
};


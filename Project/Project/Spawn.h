#pragma once
#include"ObjFile.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"Texture.h"
#include"Enemy.h"

class Spawn
{
public:
	Spawn();
	~Spawn();
	void Initialize(ID3D12Device* dev);
	void Update();
	void Draw(ID3D12GraphicsCommandList * cmdList);
	XMFLOAT3 GetPosition();
	void SetPoisition(XMFLOAT3 position);
	void SetSpawn(int amount, int time);
	Enemy* enemyamount();
	int GetAmount();
private:
	ObjFile* obj = new ObjFile();
	int amount;
	int* backamount;
	float time = 0;
	float timer = 0;
	int Cnt = 0;
	Enemy* enemy[100];
};
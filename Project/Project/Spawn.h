#pragma once
#include"Camera.h"
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
	void Stop();
	void SetSpawn(float amount, float time);
	Enemy* enemyamount();
	int GetAmount();
private:
	XMFLOAT3 texSize{ 20,50,0 };
	XMFLOAT3 texPos{ 0,0,0 };
	ObjFile* obj = new ObjFile();
	Texture* tex = new Texture();
	int amount;
	float time = 0;
	float timer = 0;
	bool stop = false;
	Enemy* enemy[100];
};